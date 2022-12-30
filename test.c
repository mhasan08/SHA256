//
// Created by Munawar Hasan
//


#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "SHA256ShortMsg.h"
#include "sha256.h"


int main(){
    int is_passed = 1;
    for (int i = 0; i < tv_length; i++){
        uint8_t buffer[MD_SIZE];
        uint8_t msg_length = TESTVECTOR[i][0][0];
        uint8_t *msg = TESTVECTOR[i][1];
        uint8_t *hash = TESTVECTOR[i][2];

        SHA256 sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, msg, msg_length);
        SHA256_Final(&sha256, buffer);
        is_passed = is_passed && !memcmp(hash, buffer, MD_SIZE);
    }
    if (is_passed)
        printf("passed!\n");
    else
        printf("failed!\n");

    return 0;
}