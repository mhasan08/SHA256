//
// Created by Munawar Hasan
//


#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "sha256.h"

uint8_t msg1[] = {0x00}; //byte array
uint8_t hash1[MD_SIZE] = {
    0xe3, 0xb0, 0xc4, 0x42, 0x98, 0xfc, 0x1c, 0x14, 
    0x9a, 0xfb, 0xf4, 0xc8, 0x99, 0x6f, 0xb9, 0x24,
    0x27, 0xae, 0x41, 0xe4, 0x64, 0x9b, 0x93, 0x4c,
    0xa4, 0x95, 0x99, 0x1b, 0x78, 0x52, 0xb8, 0x55
};


int main(){
    int is_passed = 1;

    uint8_t buffer[MD_SIZE];

    SHA256 sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, msg1, 0);
    SHA256_Final(&sha256, buffer);

    is_passed = is_passed && !memcmp(hash1, buffer, MD_SIZE);

    if (is_passed)
        printf("passed!\n");
    else
        printf("failed!\n");

    return 0;
}