/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleclair <mleclair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/11/24 11:36:08 by mleclair          #+#    #+#             */
/*   Updated: 2016/12/12 15:29:50 by mleclair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_x(va_list ap, int i, char c, char **str)
{
	unsigned long long int d;

	if (i == 1)
		d = va_arg(ap, unsigned long);
	else if (i == 2)
		d = va_arg(ap, unsigned long long);
	else if (i == 5)
		d = va_arg(ap, uintmax_t);
	else if (i == 6)
		d = va_arg(ap, size_t);
	else
		d = va_arg(ap, unsigned int);
	if (i == -1)
		d = (unsigned short)d;
	if (i == -2)
		d = (unsigned char)d;
	*str = ft_itoa_base((d > 0 ? d : -d), 16, (d > 0 ? 0 : 1));
	i = -1;
	if (c == 'x' || c == 'p')
		while ((*str)[++i])
			if ((*str)[i] >= 'A' && (*str)[i] <= 'F')
				(*str)[i] += 32;
	return (ft_strlen(*str));
}

int		ft_i(va_list ap, int i, char c, char **str)
{
	return (ft_d(ap, i, c, str));
}

int		ft_d(va_list ap, int i, char c, char **str)
{
	long long int	d;
	char			*tmp;

	if (i == 1 || c == 'D')
		d = va_arg(ap, long int);
	else if (i == 2)
		d = va_arg(ap, long long int);
	else if (i == 5)
		d = va_arg(ap, intmax_t);
	else if (i == 6)
		d = va_arg(ap, unsigned long int);
	else if (i == -1)
		d = (short)va_arg(ap, int);
	else if (i == -2)
		d = (char)va_arg(ap, int);
	else
		d = va_arg(ap, int);
	tmp = ft_itoa_base((d > 0 ? d : -d), 10, (d > 0 ? 0 : 1));
	*str = palloc(ft_strlen(tmp));
	**str = 0;
	ft_strcat(*str, tmp);
	free(tmp);
	return (ft_strlen(*str));
}

int		ft_p(va_list ap, int i, char c, char **str)
{
	i = ft_x(ap, 1, c, str);
	return (i);
}

int		ft_israndom(va_list ap, int i, char c, char **str)
{
	i = 42 * 42 / 42 - 42 + 42;
	*str = palloc(2);
	(*str)[0] = c;
	(*str)[1] = '\0';
	if (ap)
		;
	return (ft_strlen(*str));
}
