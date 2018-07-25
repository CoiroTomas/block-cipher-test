#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include "modes.h"
#include "simonalgo.h"
#include "main.h"

int main(int argc, char *argv[]){
    if (argc != 5) {
        printf("Wrong number of arguments\n");
        printHelp(argv[0]);
        return 1;
    }

    FILE *inputFile = fopen(argv[3], "rb");
    if (inputFile == NULL) {
        printf("Input file doesn't exist\n");
        return 1;
    }

    FILE *outputFile = fopen(argv[4], "wb");
    // Here I separate each mode of chaining
    if (strcmp(argv[2], "ecb") == 0) {
        // ECB = no chaining
        if (argv[1][0] == 'e') {
            encrypt_file_ecb(inputFile, outputFile, simon_encrypt);
        } else if (argv[1][0] == 'd') {
            encrypt_file_ecb(inputFile, outputFile, simon_decrypt);
        } else {
            printf("Don't know whether to encrypt or decrypt\n");
            printHelp(argv[0]);
            return 1;
        }
    } else if (strcmp(argv[2], "cbc") == 0) {
        // CBC = IV xors against plaintext, and each ciphertext chains
        if (argv[1][0] == 'e') {
            encrypt_file_cbc(inputFile, outputFile);
        } else if (argv[1][0] == 'd') {
            decrypt_file_cbc(inputFile, outputFile);
        } else {
            printf("Don't know whether to encrypt or decrypt\n");
            printHelp(argv[0]);
            return 1;
        }
    } else if (strcmp(argv[2], "cfb") == 0) {
        // CFB = IV gets encrypted, plaintext xors and ciphertext chains
        if (argv[1][0] == 'e') {
            encrypt_file_cfb(inputFile, outputFile);
        } else if (argv[1][0] == 'd') {
            decrypt_file_cfb(inputFile, outputFile);
        } else {
            printf("Don't know whether to encrypt or decrypt\n");
            printHelp(argv[0]);
            return 1;
        }
    } else if (strcmp(argv[2], "ofb") == 0) {
        // OFB = IV gets encrypted, it chains,
        // ciphertext is that xored with plaintext
        if (argv[1][0] == 'e') {
            encrypt_file_ofb(inputFile, outputFile);
        } else if (argv[1][0] == 'd') {
            decrypt_file_ofb(inputFile, outputFile);
        } else {
            printf("Don't know whether to encrypt or decrypt\n");
            printHelp(argv[0]);
            return 1;
        }
    } else {
        printf("Don't know which chaining mode to use.\n");
        printHelp(argv[0]);
    }


    if (fclose(inputFile) != 0 || fclose(outputFile) != 0) {
        printf("Something went wrong when closing the files\n");
        return 1;
    }

    return 0;
}


void printHelp(char *name) {
    printf("Usage:\n%s [encrypt|decrypt] [ecb|cbc|cfb|ofb]"
               " <input-file> <output-file>\n", name);
}
