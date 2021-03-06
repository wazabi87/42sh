/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_fd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleclair <mleclair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/02 18:38:45 by aridolfi          #+#    #+#             */
/*   Updated: 2017/05/19 17:41:23 by mleclair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chell.h"

/*
** Duplicating an Output File Descriptor: command [n]>&word
*/

static void	rd_closeoutput(t_env *env, char n)
{
	pid_t		child;
	int			std_tmp;
	int			status;

	child = -1;
	if (n == -1 || (n >= 0 && n <= 2))
		std_tmp = dup(n == -1 ? STDOUT_FILENO : (int)n);
	close(n == -1 ? STDOUT_FILENO : (int)n);
	child = fork();
	if ((int)child == -1)
		error(-16, NULL, NULL);
	else if ((int)child == 0)
	{
		parse(env, &env->input, 1);
		exit(env->lastret);
	}
	wait(&status);
	retvalue_into_loc(env, WEXITSTATUS(status));
	if (n == -1 || (n >= 0 && n <= 2))
		dup2(std_tmp, (n == -1 ? STDOUT_FILENO : (int)n));
}

void		rd_dupoutput(t_env *env, char n)
{
	pid_t		child;
	char		word;
	int			status;

	child = -1;
	word = env->redir[1] - 48;
	if (word == ('-' - 48))
	{
		rd_closeoutput(env, n);
		return ;
	}
	if (fcntl((int)word, F_GETFD) == -1)
		error(-18, NULL, NULL);
	child = fork();
	if ((int)child == -1)
		error(-16, NULL, NULL);
	else if ((int)child == 0)
	{
		dup2(word, (n == -1 ? STDOUT_FILENO : n));
		parse(env, &env->input, 1);
		exit(env->lastret);
	}
	wait(&status);
	retvalue_into_loc(env, WEXITSTATUS(status));
}
