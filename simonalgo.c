#include<stdint.h>


static uint8_t key[8] = {0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41};
static uint8_t IV[4] = {0xFE, 0xFE, 0xFE, 0xFE};

static uint16_t word_size = 16;
static uint64_t z[5] = {
    0b11111010001001010110000111001101111101000100101011000011100110,
    0b10001110111110010011000010110101000111011111001001100001011010,
    0b10101111011100000011010010011000101000010001111110010110110011,
    0b11011011101011000110010111100000010010001010011100110100001111,
    0b11010001111001101011011000100000010111000011001010010011101111,
};

static uint8_t m = 4;
static uint8_t T = 32;
static uint8_t j = 0;

uint16_t rotate_left(uint16_t word, uint8_t spaces) {
    uint16_t value = word << spaces;
    value = value | (word >> (word_size - spaces));

    return value;
}

uint16_t rotate_right(uint16_t word, uint8_t spaces) {
    uint16_t value = word >> spaces;
    value = value | (word << (word_size - spaces));

    return value;
}

void simon_round(uint32_t *block, uint16_t *key) {
    uint16_t pt1;
    uint16_t pt2;

    uint16_t *divided_block = (uint16_t*)block;

    pt1 = divided_block[0];
    pt2 = divided_block[1];

    uint16_t temp = rotate_left(pt1, 1) & rotate_left(pt1, 8);
    pt2 = temp ^ pt2;
    pt2 = pt2 ^ rotate_left(pt1, 2);
    pt2 = pt2 ^ *key;

    divided_block[0] = pt2;
    divided_block[1] = pt1;
}

void simon_encrypt(uint32_t *block, uint32_t *cipher, uint16_t *key_expansion) {
    uint32_t temp_block = *block;
    for (int i = 0; i < T; i++) {
        simon_round(&temp_block, &key_expansion[i]);
    }

    *cipher = temp_block;
}

void simon_deround(uint32_t *block, uint16_t *key) {
    uint16_t pt1;
    uint16_t pt2;

    uint16_t *divided_block = (uint16_t*)block;

    pt1 = divided_block[0];
    pt2 = divided_block[1];

    uint16_t temp;

    pt1 = pt1 ^ *key;
    pt1 = pt1 ^ rotate_left(pt2, 2);
    temp = rotate_left(pt2, 1) & rotate_left(pt2, 8);
    pt1 = temp ^ pt1;

    divided_block[1] = pt1;
    divided_block[0] = pt2;
}

void simon_decrypt(uint32_t *block, uint32_t *cipher, uint16_t *key_expansion) {
    uint32_t temp_block = *block;
    for (int i = 31; i >= 0; i--) {
        simon_deround(&temp_block, &key_expansion[i]);
    }

    *cipher = temp_block;
}

void key_generation(uint64_t *key, uint16_t *generated) {
    generated[0] = ((uint16_t*)key)[0];
    generated[1] = ((uint16_t*)key)[1];
    generated[2] = ((uint16_t*)key)[2];
    generated[3] = ((uint16_t*)key)[3];

    for (int i = 4; i < T; i++) {
        uint16_t tmp = rotate_right(generated[i - 1], 3);
        if (4 == m) {
            tmp = tmp ^ generated[i - 3];
        }
        tmp = tmp ^ rotate_right(tmp, 1);
        uint8_t bit = (z[j] >> ((i-m) % 62)) & 1;
        generated[i] = tmp ^ ~generated[i - m] ^ 3 ^ bit;
    }
}
