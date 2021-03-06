/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleclair <mleclair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/07 13:34:17 by mleclair          #+#    #+#             */
/*   Updated: 2017/05/09 20:12:51 by mleclair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chell.h"

void			queu(char *str1, char *str2)
{
	ft_putstr_fd(str1, 2);
	ft_putstr_fd(str2, 2);
	ft_putstr_fd("\n", 2);
}

static void		error3(int i)
{
	if (i == -13)
		ft_putstr_fd("\nchell: No such event\n", 2);
	else if (i == -14)
		ft_putstr_fd("exit: take only 1 numeric argument.\n", 2);
	else if (i == -15)
		ft_putstr_fd("chell: parse error\n", 2);
	else if (i == -16)
		ft_putstr_fd("chell: fork error\n", 2);
	else if (i == -17)
		ft_putstr_fd("chell: open error\n", 2);
	else if (i == -18)
		ft_putstr_fd("chell: fcntl error\n", 2);
	else if (i == -19)
		ft_putstr_fd("Wrong uses of parenthesis\n", 2);
	else if (i == -20)
		ft_putstr_fd("Buffer overload, calm down\n", 2);
}

static void		error2(int i, char *str)
{
	if (i == -7)
		ft_putstr_fd("chell: error too many arguments.\n", 2);
	else if (i == -8)
		ft_putstr_fd("chell: cd: HOME not set\n", 2);
	else if (i == -666)
	{
		ft_putstr_fd("Malloc error! ABORT! ABORT!!!!!!!!\n", 2);
		exit(EXIT_FAILURE);
	}
	else if (i == -9)
		queu("chell: cd: no such file or directory: ", str);
	else if (i == -10)
		queu("chell: cd: string not in pwd: ", str);
	else if (i == -11)
		ft_putstr_fd("chell: No history to display\n", 2);
	else if (i == -12)
	{
		ft_putstr_fd("\n", 2);
		exit(env()->lastret);
	}
	else
		error3(i);
}

void			error(int i, char *str, char *str2)
{
	retvalue_into_loc(env(), 1);
	if (str2)
		free(str2);
	if (i == -1)
		ft_putstr_fd("No such file or directory\n", 2);
	else if (i == -2)
		queu("chell: command not found: ", str);
	else if (i == -3)
		ft_putstr_fd("chell error\n", 2);
	else if (i == -4)
		queu("chell: exec format error: ", str);
	else if (i == -5)
		queu("chell: permission denied: ", str);
	else if (i == -6)
	{
		ft_putstr_fd("\n", 2);
		file_history(0, 0, 0, 0);
		exit(EXIT_SUCCESS);
	}
	else
		error2(i, str);
}
