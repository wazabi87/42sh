/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_adv.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleclair <mleclair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/06 11:55:38 by aridolfi          #+#    #+#             */
/*   Updated: 2017/05/17 14:22:00 by mleclair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chell.h"

static void	rd_delimiter(char **str)
{
	int		i;
	int		len;
	char	*tmp;

	i = -1;
	len = 0;
	while ((*str)[++i] == ' ' || (*str)[i] == '<')
		;
	while ((*str)[i + ++len] && ft_iswhitespace((*str)[i + len]) == 0)
		;
	tmp = ft_strsub(*str, i, (size_t)len);
	free_swap(&(*str), ft_strjoin(tmp, "\n"));
	free(tmp);
}

void		rd_here_doc2(t_env *env, int fd, int n)
{
	char		*buff;

	while (env->bool1 == 0 &&
	(buff = ft_strjoinfree(termcaps(ft_sprintf("heredoc> "), 9, 0), "\n", 1)))
	{
		if (env->bool1 == 1)
		{
			env->bool2 = 1;
			free(buff);
			exit(env->lastret);
		}
		if (!ft_strcmp(buff, env->inp2))
			break ;
		write(fd, buff, ft_strlen(buff));
		free(buff);
	}
	close(fd);
	if ((fd = open("/tmp/42sh-the-silence", O_RDONLY)) == -1)
		error(-17, NULL, NULL);
	unlink("/tmp/42sh-the-silence");
	dup2(fd, (n == -1 ? STDIN_FILENO : (int)n));
	parse(env, &env->inp1, 1);
	exit(env->lastret);
}

void		rd_here_doc(t_env *e, int child, int fd)
{
	char		n;
	int			status;

	n = -1;
	e->bool3 = 1;
	if (ft_isdigit(e->inp1[ft_strlen(e->inp1) ? ft_strlen(e->inp1) - 1 : 0]))
		if ((n = 0) && ft_strlen(e->inp1) > 1)
			n = (e->inp1[ft_strlen(e->inp1) - 2] == '\\' ? -1 :
			e->inp1[ft_strlen(e->inp1) - 1] - 48);
	if (n != -1)
		e->inp1[ft_strlen(e->inp1) - 1] = '\0';
	rd_delimiter(&e->inp2);
	if ((fd = open("/tmp/42sh-the-silence", O_WRONLY | O_CREAT | O_TRUNC, 0600))
	== -1)
		return (error(-17, NULL, NULL));
	child = fork();
	if ((int)child == -1)
		error(-16, NULL, NULL);
	else if ((int)child == 0)
		rd_here_doc2(e, fd, n);
	e->boolthing = child;
	wait(&status);
	retvalue_into_loc(e, WEXITSTATUS(status));
	close(fd);
	e->bool3 = 0;
}

void		rd_here_string2(t_env *env, int fd, int n)
{
	write(fd, env->inp2, ft_strlen(env->inp2));
	close(fd);
	if ((fd = open("/tmp/42sh-the-silence", O_RDONLY)) == -1)
		error(-17, NULL, NULL);
	unlink("/tmp/42sh-the-silence");
	dup2(fd, (n == -1 ? STDIN_FILENO : (int)n));
	parse(env, &env->inp1, 1);
	ft_putchar('\n');
	exit(env->lastret);
}

void		rd_here_string(t_env *e, int fd, int n, pid_t child)
{
	char		**s;
	int			status;

	if (ft_isdigit(e->inp1[ft_strlen(e->inp1) ? ft_strlen(e->inp1) - 1 : 0]))
		if ((n = 0) && ft_strlen(e->inp1) > 1)
			n = (e->inp1[ft_strlen(e->inp1) - 2] == '\\' ? -1
			: e->inp1[ft_strlen(e->inp1) - 1] - 48);
	if (n != -1)
		e->inp1[ft_strlen(e->inp1) - 1] = '\0';
	if ((s = ft_strsplitquote(e->inp2, ' ', 1)) && !s[0])
		return (free(s));
	ft_suppr_quotes(s, 0, 0);
	free_swap(&e->inp2, ft_strdup(s[0]));
	free_double_array(s);
	if ((fd = open("/tmp/42sh-the-silence", O_WRONLY | O_CREAT | O_TRUNC, 0600))
	== -1)
		error(-17, NULL, NULL);
	child = fork();
	if ((int)child == -1)
		error(-16, NULL, NULL);
	else if ((int)child == 0)
		rd_here_string2(e, fd, n);
	wait(&status);
	retvalue_into_loc(e, WEXITSTATUS(status));
	close(fd);
}
