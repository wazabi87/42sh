/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfrochot <bfrochot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/09 19:16:47 by bfrochot          #+#    #+#             */
/*   Updated: 2017/05/15 15:20:47 by bfrochot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chell.h"

int		save_env(t_env *env)
{
	if (env->savev)
	{
		free_double_array(env->ev);
		env->ev = env->savev;
		env->savev = NULL;
	}
	return (1);
}

void	print_split(char **split)
{
	int i;

	i = -1;
	while (split[++i])
	{
		ft_putstr(split[i]);
		ft_putchar('\n');
	}
}

char	**ft_cpy_double_array(char **a)
{
	int		i;
	char	**b;

	i = -1;
	while (a[++i])
		;
	b = palloc(sizeof(char*) * (i + 1));
	b[i] = 0;
	while (i--)
		b[i] = ft_strdup(a[i]);
	return (b);
}

void	reco_env_options(t_env *env, char **split, int *i)
{
	char	c;
	int		j;

	while (split[++*i] && split[*i][0] == '-')
	{
		j = 0;
		while (split[*i][++j])
			if (split[*i][j] == 'i')
				c = 1;
			else
			{
				ft_putstr("BAD OPTION. I ONLY TAKE -i YOU ARSEHOLE!\n");
				return ;
			}
	}
	if (env->savev == NULL)
		env->savev = ft_cpy_double_array(env->ev);
	if (c == 1)
	{
		free_double_array(env->ev);
		env->ev = palloc(sizeof(char **));
		env->ev[0] = 0;
	}
}

void	reco_env(t_env *env, char **split, int j, int i)
{
	int		k;
	char	*s;
	char	*tmp;

	reco_env_options(env, split, &i);
	while (split[i] && (j = -1))
	{
		while (split[i][++j] && split[i][j] != '=')
			;
		if (split[i][j] == '=')
			add_var_to_env(env, split[i]);
		else
		{
			k = ft_strfind(env->input, 'v');
			s = ft_strstr(env->input + k, split[i]);
			tmp = env->input;
			env->input = ft_strdup((*s == '\'' || *s == '"') ? s - 1 : s);
			free(tmp);
			ft_reco_cmd(env, 0);
			break ;
		}
		++i;
	}
	if (!split[i])
		print_split(env->ev);
}
