#ifndef SIMONALGO_H
#define SIMONALGO_H

#include<stdint.h>

uint8_t key[8];
uint8_t IV[4];
void simon_encrypt(uint32_t *block, uint32_t *cipher, uint16_t *key_expansion);
void simon_decrypt(uint32_t *block, uint32_t *cipher, uint16_t *key_expansion);
void key_generation(uint64_t *key, uint16_t *generated);

#endif // SIMONALGO_H
