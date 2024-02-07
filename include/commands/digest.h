#ifndef DIGEST_H
# define DIGEST_H

#include "stdbool.h"
#include "libft_core.h"
#include "utils.h"

typedef struct s_digest_options {
    bool pass;
    bool quiet;
    bool reverse;
    char *str;
    char **files;
}   t_digest_options;

int     parser_digest(char* cmd, int nbr_token, char **tokens, void **p_options);
void    free_digest(void *options);

// Digest commands
int exec_md5(void* options);

#endif