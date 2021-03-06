/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   operator.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cosi <cosi@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/22 13:43:33 by aridolfi          #+#    #+#             */
/*   Updated: 2017/05/14 01:19:16 by cosi             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chell.h"

/*
** AND Lists: command1 [&& command2] ...
*/

void	oprt_and(t_env *env)
{
	pid_t	child;
	int		status;
	char	*tmp;

	child = -1;
	child = fork();
	if ((int)child == -1)
		error(-16, NULL, NULL);
	else if ((int)child == 0)
	{
		tmp = ft_strdup(env->inp1);
		parse(env, &tmp, 1);
		free(tmp);
		exit(env->lastret);
	}
	waitpid(child, &status, 0);
	if (WEXITSTATUS(status) == 0)
	{
		parse(env, &env->inp2, 1);
		retvalue_into_loc(env, env->lastret);
	}
	else
		retvalue_into_loc(env, WEXITSTATUS(status));
}

/*
** OR Lists: command1 [|| command2] ...
*/

void	oprt_or(t_env *env)
{
	pid_t	child;
	int		status;
	char	*tmp;

	child = -1;
	child = fork();
	if ((int)child == -1)
		error(-16, NULL, NULL);
	else if ((int)child == 0)
	{
		tmp = ft_strdup(env->inp1);
		parse(env, &tmp, 1);
		exit(env->lastret);
		free(tmp);
	}
	waitpid(child, &status, 0);
	if (WEXITSTATUS(status) != 0)
	{
		parse(env, &env->inp2, 1);
		env->isoperand = 0;
		retvalue_into_loc(env, env->lastret);
	}
	else
		retvalue_into_loc(env, WEXITSTATUS(status));
}
