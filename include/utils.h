#ifndef UTILS_H
# define UTILS_H

#include "get_next_line.h"
#include "ft_printf.h"

#define WHITE_SPACE " \t\r\n\v\f"

#define ERROR_PRINT(exit_status, error_msg, ...) {ft_fprintf(2, error_msg, ##__VA_ARGS__); return exit_status;}

void free_split(char **cmd_split);
// Create malloc and copy the string
char *ft_str_mem_cpy(char *str);

// Scan file descriptor until EOF is found (returns 0 if memory allocation fails)
char *scan_fd(int fd);

#endif