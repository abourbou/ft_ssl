#ifndef DIGEST_H
# define DIGEST_H

#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <byteswap.h>

#include "stdbool.h"
#include "libft_core.h"
#include "utils.h"
#include "utils_crypto.h"

typedef struct s_digest_options {
    bool pass;
    bool quiet;
    bool reverse;
    char *str;
    char **files;
}   t_digest_options;

typedef int(*digest_algo)(uint8_t *hash, int fd, char *str);

// Digest functions
int     parser_digest(char* cmd, int nbr_token, char **tokens, void **p_options);
void    free_digest(void *options);
int     exec_digest(char *name_algo, void* options, digest_algo algo, size_t len_hash);
void    digest_print_help(char *name_algo);

// Print functions
void print_hash(uint8_t* hash, int len);
void print_hash_newline(uint8_t* hash, int len);
void print_stdin_hash(uint8_t* hash, int len, char *stdin);
void print_str_hash(uint8_t* hash, int len, char *algo, char *str, bool rev);
void print_file_hash(uint8_t* hash, int len, char *algo, char *str, bool rev);

// md5
#define MD5_BUFF_SIZE 1024 // Need to be at least 256
#define F_MD5(B,C,D) ((B & C) | (~B & D))
#define G_MD5(B,C,D) ((B & D) | (C & ~D))
#define H_MD5(B,C,D) (B ^ C ^ D)
#define I_MD5(B,C,D) (C ^ (B | ~D))
int     exec_md5(void* options);
void    print_help_md5(void);

// sha256
#define SHA256_BUFF_SIZE 1024 // Need to be at least 256
int exec_sha256(void* options);
void print_help_sha256(void);

// whirlpool
#define WHIRLPOOL_BUFF_SIZE 1024 // Need to be at least 256
int exec_whirlpool(void* options);
void print_help_whirlpool(void);
const uint64_t *get_C_whirlpool(int i);
const uint64_t *get_rc_whirlpool();

#endif