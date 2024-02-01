/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   get_next_line.h                                  .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: abourbou <abourbou@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/10/29 13:32:26 by abourbou     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/03 13:45:41 by abourbou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H
# include "../include/libft_core.h"

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 64
# endif

typedef	struct	s_list_fd
{
	int					fd;
	char				*reminder;
	struct s_list_fd	*next;
}				t_list_fd;

int				get_next_line(int fd, char **line);
void			free_gnl_fd(int fd);
void			free_gnl(void);


// Private functions
int				ft_lstclear_fd(t_list_fd **lst);
int				ft_lstdelone_fd(t_list_fd **first_elem, int fd);
int				ft_strcut_gnl(char **reminder, char **line);
#endif
