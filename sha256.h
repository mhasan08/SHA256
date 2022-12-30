//
// Created by Munawar Hasan
//


#ifndef SHA256_SHA256_H
#define SHA256_SHA256_H

#include <stdint.h>

#define MD_SIZE 32            // SHA256 output digest size

typedef struct {
    uint8_t data[64];
    uint32_t datalen;
    uint64_t l;
    uint32_t state[8];
}SHA256;

void SHA256_Init(SHA256 *);
void SHA256_Update(SHA256 *, const uint8_t *, uint64_t);
void SHA256_Final(SHA256 *, uint8_t *);

#endif //SHA256_SHA256_H
