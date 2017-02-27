/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleclair <mleclair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/27 13:01:10 by mleclair          #+#    #+#             */
/*   Updated: 2017/02/27 13:01:26 by mleclair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "42sh.h"

/*
** Pipelines: command1 | command2
*/

static void	frk_pipe(t_env *env)
{
<<<<<<< Updated upstream
	pid_t		child;
	int			fds[2];
	int const	READ_END = 0;
	int const	WRITE_END = 1;
=======
	pid_t	child;
	int		fds[2];
>>>>>>> Stashed changes

	child = -1;
	pipe(fds);
	child = fork();
	if ((int)child == -1)
	{
		close(fds[READ_END]);
		close(fds[WRITE_END]);
		perror("error");
	}
	else if ((int)child == 0)
	{
		dup2(fds[READ_END], STDOUT_FILENO);
		close(fds[WRITE_END]);
		parse(env, env->inp1);
		exit(EXIT_SUCCESS);
	}
	dup2(fds[WRITE_END], STDIN_FILENO);
	close(fds[READ_END]);
	wait(NULL);
	parse(env, env->inp2);
}

void	rd_pipe(t_env *env)
{
	pid_t	child;

	child = -1;
	child = fork();
	if ((int)child == -1)
	{
		perror("error");
		exit(EXIT_FAILURE);
	}
	else if ((int)child == 0)
	{
		frk_pipe(env);
		exit(EXIT_SUCCESS);
	}
	wait(NULL);
}

/*
** Redirecting Output: command > output.txt
*/

void		rd_output(t_env *env, int i)
{
	pid_t	child;
	int		fd;
	char	**s;

	child = -1;
	fd = -1;
	s = ft_strsplitquote(env->redir[i], ' ', 0);
	if ((fd = open(s[1] == 0 ? s[0] + 1 : s[1], O_WRONLY | O_CREAT | O_TRUNC, 0644)) == -1)
		perror("error");
	free_double_array(s);
	child = fork();
	if ((int)child == -1)
	{
		close(fd);
		perror("error");
	}
	else if ((int)child == 0)
	{
		dup2(fd, STDOUT_FILENO);
		parse(env, env->input);
		exit(EXIT_SUCCESS);
	}
	close(fd);
	wait(NULL);
}

/*
** Appending Redirected Output: command >> output.txt
*/

void		rd_output_apd(t_env *env, int i)
{
	pid_t		child;
	int			fd;
	char	**s;

	child = -1;
	fd = -1;
	s = ft_strsplitquote(env->redir[i], ' ', 0);
	if ((fd = open(s[1] == 0 ? s[0] + 2 : s[1], O_WRONLY | O_CREAT | O_APPEND, 0644)) == -1)
		perror("error");
	free_double_array(s);
	child = fork();
	if ((int)child == -1)
	{
		close(fd);
		perror("error");
	}
	else if ((int)child == 0)
	{
		dup2(fd, STDOUT_FILENO);
		parse(env, env->input);
		exit(EXIT_SUCCESS);
	}
	close(fd);
	wait(NULL);
}

/*
** Redirecting Input: command < output.txt
*/

void		rd_input(t_env *env)
{
	pid_t		child;
	int			fd;

	child = -1;
	fd = -1;
	if ((fd = open(env->inp2, O_RDONLY)) == -1)
		perror("error");
	child = fork();
	if ((int)child == -1)
	{
		close(fd);
		perror("error");
	}
	else if ((int)child == 0)
	{
		dup2(fd, STDIN_FILENO);
		parse(env, env->inp1);
		exit(EXIT_SUCCESS);
	}
	close(fd);
	wait(NULL);
}

/*
** Here-Document:
**
** interactive-program << delimiter
** command 1
** command 2
** .........
** command n
** delimiter
**
** command <<- delimiter
** 		here type your
**		input tabulation
**		will be ignored
** delimiter
**
*/

static void	rd_delimiter(char **str)
{
	int i;

	i = -1;
	while ((*str)[++i] == ' ' || (*str)[i] == '<')
		;
	*str[0] = 0;
	ft_strcat(*str, *str + i);
	ft_strcat(*str, "\n");
}

void		rd_here_doc(t_env *env)
{
	pid_t		child;
	int			fd;
	int			rsize;
	char		*buff;

	child = -1;
	fd = -1;
	rsize = -1;
	rd_delimiter(&env->inp2);
	if ((fd = open("/tmp/42sh-the-silence", O_WRONLY | O_CREAT | O_TRUNC, 0600)) == -1)
		perror("error");
	child = fork();
	if ((int)child == -1)
	{
		close(fd);
		perror("error");
	}
	else if ((int)child == 0)
	{
		while ((buff = ft_strjoin(termcaps(ft_sprintf("heredoc> ")), "\n")))
		{
			if (!ft_strcmp(buff, env->inp2))
				break;
			write(fd, buff, ft_strlen(buff));
			free(buff);
		}
		close(fd);
		if ((fd = open("/tmp/42sh-the-silence", O_RDONLY)) == -1)
			perror("error");
		unlink("/tmp/42sh-the-silence");
		dup2(fd, STDIN_FILENO);
		parse(env, env->inp1);
		exit(EXIT_SUCCESS);
	}
	wait(NULL);
	close(fd);
}
