/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   ft_uns_putnbr.c                                  .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: abourbou <abourbou@student.le-101.fr>      +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/11/12 12:54:08 by abourbou     #+#   ##    ##    #+#       */
/*   Updated: 2020/02/03 15:20:25 by abourbou    ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "../include/ft_printf.h"

extern int g_ft_printf_fd;

static void	write_nb(unsigned int nb)
{
	char	buff;

	if (nb <= 9)
	{
		buff = nb + '0';
		write(g_ft_printf_fd, &buff, 1);
	}
	else
	{
		write_nb(nb / 10);
		write_nb(nb % 10);
	}
}

void		ft_uns_putnbr(unsigned int nb)
{
	if (nb == 0)
	{
		write(g_ft_printf_fd, "0", 1);
		return ;
	}
	write_nb(nb);
}

static int	write_nb_compt(unsigned int nb)
{
	int	ret;

	ret = 0;
	if (nb <= 9)
		ret++;
	else
	{
		ret += write_nb_compt(nb / 10);
		ret += write_nb_compt(nb % 10);
	}
	return (ret);
}

int			ft_uns_putnbr_compt(unsigned int nb)
{
	if (nb == 0)
		return (1);
	return (write_nb_compt(nb));
}
