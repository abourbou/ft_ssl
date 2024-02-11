#ifndef UTILS_H
# define UTILS_H

#include "get_next_line.h"
#include "ft_printf.h"

#define WHITE_SPACE " \t\r\n\v\f"

#define ERROR_PRINT(exit_status, error_msg, ...) {ft_fprintf(2, error_msg, ##__VA_ARGS__); return exit_status;}

#define UTILS_BUFF_SIZE 1024

void free_split(char **cmd_split);
int free_and_return(int val, void *buffer);

// Create malloc and copy the string
char *ft_str_mem_cpy(char *str);

// Scan file descriptor until EOF is found (returns 0 if memory allocation fails)
char *scan_fd(int fd);

void    print_memory_bin(void *data, size_t len);

void    print_memory_hex(void *data, size_t len);

#endif