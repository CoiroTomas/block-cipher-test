#ifndef MODES_H
#define MODEs_H

void pass_header(FILE *input, FILE *output);
void printHelp(char *name);
void encrypt_file_ecb(FILE *input, FILE *output,
    void (*function) (uint32_t*, uint32_t*, uint16_t*));
void encrypt_file_cbc(FILE *input, FILE *output);
void decrypt_file_cbc(FILE *input, FILE *output);
void encrypt_file_cfb(FILE *input, FILE *output);
void decrypt_file_cfb(FILE *input, FILE *output);
void encrypt_file_ofb(FILE *input, FILE *output);
void decrypt_file_ofb(FILE *input, FILE *output);

#endif // MODES_H
