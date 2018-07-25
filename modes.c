#include<stdio.h>
#include<math.h>
#include "simonalgo.h"
#include "modes.h"

void encrypt_file_ecb(FILE *input, FILE *output,
    void (*function) (uint32_t*, uint32_t*, uint16_t*)) {
    pass_header(input, output);

    uint16_t key_exp[32];
    key_generation((uint64_t*) key, key_exp);
    uint32_t block;
    uint32_t ciphered;
    while (fread(&block, sizeof(block), 1, input)) {
        function(&block, &ciphered, key_exp);
        fwrite(&ciphered, sizeof(block), 1, output);
    }
    size_t bytes = fread(&block, 1, 4, input);
    fwrite(&block, 1, bytes, output);
}

void encrypt_file_cbc(FILE *input, FILE *output) {
	pass_header(input, output);
	
	uint32_t current_IV = *(uint32_t*)IV;
	
	uint16_t key_exp[32];
	key_generation((uint64_t*) key, key_exp);
	uint32_t block;
	uint32_t ciphered;
	while (fread(&block, sizeof(block), 1, input)) {
		block = block ^ current_IV;
		
		simon_encrypt(&block, &ciphered, key_exp);
		fwrite(&ciphered, sizeof(block), 1, output);
		
		current_IV = ciphered;
	}
	size_t bytes = fread(&block, 1, 4, input);
    fwrite(&block, 1, bytes, output);
}

void decrypt_file_cbc(FILE *input, FILE *output) {
	pass_header(input, output);
	
	uint32_t current_IV = *(uint32_t*)IV;
	
	uint16_t key_exp[32];
	key_generation((uint64_t*) key, key_exp);
	uint32_t block;
	uint32_t ciphered;
	while (fread(&block, sizeof(block), 1, input)) {
		simon_decrypt(&block, &ciphered, key_exp);
		
		ciphered = ciphered ^ current_IV;
		fwrite(&ciphered, sizeof(block), 1, output);
		
		current_IV = block;
	}
	size_t bytes = fread(&block, 1, 4, input);
    fwrite(&block, 1, bytes, output);
}

void encrypt_file_cfb(FILE *input, FILE *output) {
	pass_header(input, output);
	
	uint32_t current_IV = *(uint32_t*)IV;
	
	uint16_t key_exp[32];
    key_generation((uint64_t*) key, key_exp);
	uint32_t block;
	uint32_t ciphered;
	while (fread(&block, sizeof(block), 1, input)) {
		simon_encrypt(&current_IV, &ciphered, key_exp);
		
		ciphered = ciphered ^ block;
		
		fwrite(&ciphered, sizeof(block), 1, output);
		
		current_IV = ciphered;
	}
	size_t bytes = fread(&block, 1, 4, input);
    fwrite(&block, 1, bytes, output);
}

void decrypt_file_cfb(FILE *input, FILE *output) {
	pass_header(input, output);
	
	uint32_t current_IV = *(uint32_t*)IV;
	
	uint16_t key_exp[32];
    key_generation((uint64_t*) key, key_exp);
	uint32_t block;
	uint32_t ciphered;
	while (fread(&block, sizeof(block), 1, input)) {
		simon_encrypt(&current_IV, &ciphered, key_exp);
		
		ciphered = ciphered ^ block;
		
		fwrite(&ciphered, sizeof(block), 1, output);
		
		current_IV = block;
	}
	size_t bytes = fread(&block, 1, 4, input);
    fwrite(&block, 1, bytes, output);
}

void encrypt_file_ofb(FILE *input, FILE *output) {
	pass_header(input, output);
	
	uint32_t current_IV = *(uint32_t*)IV;
	
	uint16_t key_exp[32];
    key_generation((uint64_t*) key, key_exp);
	uint32_t block;
	uint32_t ciphered;
	while (fread(&block, sizeof(block), 1, input)) {
		simon_encrypt(&current_IV, &ciphered, key_exp);
		
		current_IV = ciphered;
		ciphered = ciphered ^ block;
		
		fwrite(&ciphered, sizeof(block), 1, output);
	}
	size_t bytes = fread(&block, 1, 4, input);
    fwrite(&block, 1, bytes, output);
}

void decrypt_file_ofb(FILE *input, FILE *output) {
	pass_header(input, output);
	
	uint32_t current_IV = *(uint32_t*)IV;
	
	uint16_t key_exp[32];
    key_generation((uint64_t*) key, key_exp);
	uint32_t block;
	uint32_t ciphered;
	while (fread(&block, sizeof(block), 1, input)) {
		simon_encrypt(&current_IV, &ciphered, key_exp);
		
		current_IV = ciphered;
		ciphered = ciphered ^ block;
		
		fwrite(&ciphered, sizeof(block), 1, output);
	}
	size_t bytes = fread(&block, 1, 4, input);
    fwrite(&block, 1, bytes, output);
}

void pass_header(FILE *input, FILE *output) {
	uint8_t tmp = 0;
    uint32_t file_ptr = 0;
    
    for(int i = 0; i < 10; i++) {
		// Copy the header before the offset
        file_ptr++;
        fread(&tmp, sizeof(tmp), 1, input);
        fwrite(&tmp, sizeof(tmp), 1, output);
    }
    
    uint32_t offset = 0;
    fread(&offset, sizeof(offset), 1, input);
    fwrite(&offset, sizeof(offset), 1, output);
    file_ptr += 4;

    while(file_ptr < offset) {
		// Finish the header before reaching the image
        file_ptr++;
        fread(&tmp, sizeof(tmp), 1, input);
        fwrite(&tmp, sizeof(tmp), 1, output);
    }
}
