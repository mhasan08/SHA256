//
// Created by Munawar Hasan
//

// https://en.wikipedia.org/wiki/SHA-2

#include "sha256.h"

#define RIGHTROTATE(a,b) ((a >> b) | (a << (32-b)))

#define CH(x,y,z) ((x & y) ^ (~x & z))
#define MAJ(x,y,z) ((x & y) ^ (x & z) ^ (y & z))
#define SIGMA0(x) (RIGHTROTATE(x, 2) ^ RIGHTROTATE(x, 13) ^ RIGHTROTATE(x, 22))
#define SIGMA1(x) (RIGHTROTATE(x, 6) ^ RIGHTROTATE(x, 11) ^ RIGHTROTATE(x, 25))

static const uint32_t k[64] = {
        0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
        0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
        0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
        0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
        0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
        0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
        0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
        0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

void SHA256_Compression(SHA256 *sha256, const uint8_t *data){
    uint32_t a, b, c, d, e, f, g, h;
    uint32_t i, j ;
    uint32_t temp1, temp2;
    uint32_t msg[64];

    for (i = 0, j = 0; i < 16; i++, j+=4)
        msg[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
    for (i = 16; i < 64; i++) {
        msg[i] = (
                msg[i - 16]
                + (RIGHTROTATE(msg[i-15],7) ^ RIGHTROTATE(msg[i-15],18) ^ (msg[i-15] >> 3))
                + msg[i - 7]
                + (RIGHTROTATE(msg[i - 2],17) ^ RIGHTROTATE(msg[i - 2],19) ^ (msg[i - 2] >> 10))
                );
    }

    a = sha256->state[0];
    b = sha256->state[1];
    c = sha256->state[2];
    d = sha256->state[3];
    e = sha256->state[4];
    f = sha256->state[5];
    g = sha256->state[6];
    h = sha256->state[7];

    for (i = 0; i < 64; i++) {
        temp1 = h + SIGMA1(e) + CH(e,f,g) + k[i] + msg[i];
        temp2 = SIGMA0(a) + MAJ(a,b,c);
        h = g;
        g = f;
        f = e;
        e = d + temp1;
        d = c;
        c = b;
        b = a;
        a = temp1 + temp2;
    }

    sha256->state[0] += a;
    sha256->state[1] += b;
    sha256->state[2] += c;
    sha256->state[3] += d;
    sha256->state[4] += e;
    sha256->state[5] += f;
    sha256->state[6] += g;
    sha256->state[7] += h;

}

void SHA256_Update(SHA256 *sha256, const uint8_t *data, uint64_t l){
    uint32_t i;
    for (i = 0; i < l; i++){
        sha256->data[sha256->datalen] = data[i];
        sha256->datalen += 1;
        if (sha256->datalen == 64){
            SHA256_Compression(sha256, sha256->data);
            sha256->l+= 512;
            sha256->datalen = 0;
        }
    }
}

void SHA256_Final(SHA256 *sha256, uint8_t *hash){
    uint32_t data_length = sha256->datalen;
    uint32_t i;

    // Padding
    if (sha256->datalen < 56) {
        sha256->data[data_length++] = 0x80;
        while (data_length < 56)
            sha256->data[data_length++] = 0x00;
    } else{
        sha256->data[data_length++] = 0x80;
        while (data_length < 64)
            sha256->data[data_length++] = 0x00;
        SHA256_Compression(sha256, sha256->data);

        // build next message for compression
        for (i = 0; i < 56; i++)
            sha256->data[i] = 0x00;
    }

    // Append message's length and compress
    sha256->l += sha256->datalen * 8;
    sha256->data[63] = sha256->l;
    sha256->data[62] = sha256->l >> 8;
    sha256->data[61] = sha256->l >> 16;
    sha256->data[60] = sha256->l >> 24;
    sha256->data[59] = sha256->l >> 32;
    sha256->data[58] = sha256->l >> 40;
    sha256->data[57] = sha256->l >> 48;
    sha256->data[56] = sha256->l >> 56;
    SHA256_Compression(sha256, sha256->data);

    // little endian to big endian
    for (i = 0; i < 4; i++) {
        hash[i]      = (sha256->state[0] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 4]  = (sha256->state[1] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 8]  = (sha256->state[2] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 12] = (sha256->state[3] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 16] = (sha256->state[4] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 20] = (sha256->state[5] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 24] = (sha256->state[6] >> (24 - i * 8)) & 0x000000ff;
        hash[i + 28] = (sha256->state[7] >> (24 - i * 8)) & 0x000000ff;
    }
}

void SHA256_Init(SHA256 *sha256){
    sha256->datalen = 0;
    sha256->l = 0;

    sha256->state[0] = 0x6a09e667;
    sha256->state[1] = 0xbb67ae85;
    sha256->state[2] = 0x3c6ef372;
    sha256->state[3] = 0xa54ff53a;
    sha256->state[4] = 0x510e527f;
    sha256->state[5] = 0x9b05688c;
    sha256->state[6] = 0x1f83d9ab;
    sha256->state[7] = 0x5be0cd19;
}