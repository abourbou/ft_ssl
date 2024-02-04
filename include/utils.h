#ifndef UTILS_H
# define UTILS_H

#include "ft_printf.h"

#define WHITE_SPACE " \t\r\n\v\f"

#define ERROR_PRINT(exit_status, error_msg, ...) {ft_fprintf(2, error_msg, ##__VA_ARGS__); return exit_status;}

void free_split(char **cmd_split);

#endif