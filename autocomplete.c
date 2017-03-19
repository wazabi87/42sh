/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   autocomplete.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bfrochot <bfrochot@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/02/10 15:01:14 by bfrochot          #+#    #+#             */
/*   Updated: 2017/03/17 15:41:20 by bfrochot         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chell.h"

char	*add_bs(char *str)
{
	int		i;
	char	*tmp;

	i = -1;
	while (str[++i])
	{
		if (str[i] == '\\' || str[i] == ' ' || str[i] == '\t' || str[i] == '>'
		|| str[i] == '<' || str[i] == '"' || str[i] == '\'' || str[i] == ';' ||
		str[i] == '|' || str[i] == '&' || str[i] == '$' || str[i] == '*')
		{
			tmp = palloc(ft_strlen(str) + 2);
			*tmp = 0;
			ft_strcat(tmp, str);
			tmp[i] = '\\';
			tmp[i + 1] = 0;
			ft_strcat(tmp, str + i);
			if (str[i + 1])
				++i;
			free(str);
			str = tmp;
		}
	}
	return (str);
}

int		strstr_bool(char *find, char *search_in_lc)
{
	int			i;
	int			j;
	long int	f_len;

	if ((f_len = ft_strlen(find)) == 0)
		return (1);
	i = 0;
	while (search_in_lc[i])
		if (search_in_lc[i] == find[0])
		{
			j = -1;
			while (search_in_lc[i] && search_in_lc[i] == find[++j])
				++i;
			if (f_len == j)
			{
				free(search_in_lc);
				return (1);
			}
		}
		else
			++i;
	free(search_in_lc);
	return (0);
}

char	*to_lwcase(char *str)
{
	int		i;
	char	*str_lc;

	str_lc = ft_strdup(str);
	i = -1;
	while (str_lc[++i])
		if (str_lc[i] >= 'A' && str_lc[i] <= 'Z')
			str_lc[i] = str_lc[i] - 'A' + 'a';
	return (str_lc);
}

void	add_str_to_dstr(char ***dstr, char *str)
{
	int		i;
	char	**new_dtr;

	i = 0;
	while ((*dstr)[i])
		++i;
	new_dtr = palloc(sizeof(char *) * (i + 2));
	i = 0;
	while ((*dstr)[i])
	{
		new_dtr[i] = (*dstr)[i];
		++i;
	}
	new_dtr[i] = ft_strdup(str);
	new_dtr[i + 1] = 0;
	free(*dstr);
	*dstr = new_dtr;
}

void	ft_ac_cmd_build(char ***ac, char *find)
{
	if (strstr_bool(find, ft_strdup("cd")))
		add_str_to_dstr(ac, "cd");
	if (strstr_bool(find, ft_strdup("aperture")))
		add_str_to_dstr(ac, "aperture");
	if (strstr_bool(find, ft_strdup("unsetenv")))
		add_str_to_dstr(ac, "unsetenv");
	if (strstr_bool(find, ft_strdup("setenv")))
		add_str_to_dstr(ac, "setenv");
	if (strstr_bool(find, ft_strdup("exit")))
		add_str_to_dstr(ac, "exit");
	if (strstr_bool(find, ft_strdup("Patate")))
		add_str_to_dstr(ac, "Patate");
	if (strstr_bool(find, ft_strdup("env")))
		add_str_to_dstr(ac, "env");
	if (strstr_bool(find, ft_strdup("local")))
		add_str_to_dstr(ac, "local");
	if (strstr_bool(find, ft_strdup("unset")))
		add_str_to_dstr(ac, "unset");
	if (strstr_bool(find, ft_strdup("export")))
		add_str_to_dstr(ac, "export");
	if (strstr_bool(find, ft_strdup("set")))
		add_str_to_dstr(ac, "set");
	if (strstr_bool(find, ft_strdup("history")))
		add_str_to_dstr(ac, "history");
}

void	ft_ac_cmd_path(char **split_path, char *find, char ***ac)
{
	DIR			*dir;
	t_dirent	*dirent;
	int			i;
	char		*str;

	i = -1;
	while (split_path[++i])
		if ((dir = opendir(split_path[i])))
		{
			while ((dirent = readdir(dir)))
			{
				str = add_bs(ft_strdup(dirent->d_name));
				if (strstr_bool(find, to_lwcase(str)))
					add_str_to_dstr(ac, str);
				free(str);
			}
			closedir(dir);
		}
}

char	**ac_cmd(char *find, t_env *env)
{
	char		**ac;
	char		**split_path;
	int			i;

	ac = palloc(sizeof(char *));
	ac[0] = NULL;
	split_path = NULL;
	if ((i = find_param(env->ev, "PATH")) == -1)
	{
		if ((i = find_param(env->loc->ev, "PATH")) != -1)
			split_path = ft_strsplitquote(env->loc->ev[i], ':', 0);
	}
	else
		split_path = ft_strsplitquote(env->ev[i], ':', 0);
	if (split_path)
		ft_ac_cmd_path(split_path, find, &ac);
	ft_ac_cmd_build(&ac, find);
	return (ac);
}

char	**ac_pwd(char *find, int count, char *str, int i)
{
	DIR			*dir;
	t_dirent	*td;
	char		**sug;
	char		**new;

	sug = palloc(sizeof(char *));
	sug[0] = 0;
	getcwd(str, INPUT_SIZE);
	dir = opendir(str);
	while ((td = readdir(dir)))
		if (strstr_bool(find, add_bs(to_lwcase(td->d_name))) && td->d_name[0] != '.')
		{
			++count;
			new = palloc(sizeof(char *) * (count + 1));
			i = -1;
			while (sug[++i])
				new[i] = sug[i];
			new[i] = add_bs(ft_strdup(td->d_name));
			new[i + 1] = 0;
			free(sug);
			sug = new;
		}
	closedir(dir);
	free(str);
	return (sug);
}

void	ac_target2(char *after_path, t_dirent *td, char *find, char ***ac)
{
	int		i;
	int		len;
	char	**new;
	char	*tmp;

	len = 0;
	while ((*ac)[len])
		++len;
	tmp = add_bs(ft_strdup(td->d_name));
	if (strstr_bool(after_path, to_lwcase(tmp)) && td->d_name[0] != '.')
	{
		new = palloc(sizeof(char *) * (len + 2));
		i = -1;
		while ((*ac)[++i])
			new[i] = (*ac)[i];
		new[i] = malloc(ft_strlen(find) + ft_strlen(tmp) + 1);
		new[i][0] = 0;
		ft_strcat(new[i], find);
		ft_strcat(new[i], tmp);
		new[i + 1] = 0;
		free(*ac);
		(*ac) = new;
	}
	free(tmp);
}

void	ac_target(char *find, char ***ac)
{
	char		*after_path;
	int			i;
	int			j;
	DIR			*dir;
	t_dirent	*dirent;

	*ac = palloc(sizeof(char *) * 2);
	(*ac)[0] = 0;
	(*ac)[1] = 0;
	i = ft_strlen(find);
	while (!bs_str(find, i, '/'))
		--i;
	after_path = palloc(ft_strlen(find) - i);
	j = -1;
	while (find[++i])
		after_path[++j] = find[i];
	after_path[j + 1] = 0;
	find[i - j - 1] = 0;
	if ((dir = opendir(find)))
		while ((dirent = readdir(dir)))
			ac_target2(after_path, dirent, find, ac);
	if (dir)
		closedir(dir);
}

char	**auto_possibilities(char pwd, t_env *ev)
{
	char	**ac;
	char	*find_lwc;
	char	*save;
	DIR		*dir;

	find_lwc = ev->input;
	ev->input = ev->find;
	ft_dollar(ev, -1, 0);
	ev->find = ev->input;
	ev->input = find_lwc;
	save = ft_strdup(ev->find);
	if (!bs_str(ev->find, ft_strlen(ev->find) - 1, '/')
		&& (dir = opendir(ev->find)))
	{
		ac = malloc(sizeof(char *) * 2);
		ac[0] = ft_strdup(ft_strcat(ev->find, "/"));
		ac[1] = 0;
		closedir(dir);
		return (ac);
	}
	if (ev->find[0] == '/' || ev->find[0] == '.' || (dir = opendir(ev->find)))
	{
		ac_target(save, &ac);
		if (ac[0] == 0 && dir)
		{
			ac[0] = ft_strdup(ev->find);
			closedir(dir);
		}
		return (ac);
	}
	free(save);
	find_lwc = to_lwcase(ev->find);
	if (pwd == 0)
		ac = ac_cmd(find_lwc, ev);
	else
		ac = ac_pwd(find_lwc, 0, palloc(INPUT_SIZE), 0);
	return (ac);
}

char	*finder(char *input, int pos)
{
	char	*find;
	int		i;
	int		j;

	while (pos != 0 && !bs_str(input, pos, ' ') && !bs_str(input, pos, '\t'))
		pos--;
	i = pos == 0 ? 0 : pos + 1;
	j = 0;
	while (input[i] && !bs_str(input, i++, ' '))
		++j;
	find = malloc(j + 2);
	find[j] = 0;
	find[j + 1] = 0;
	i = 0;
	pos = pos == 0 ? pos - 1 : pos;
	while (input[++pos] && !bs_str(input, pos, ' '))
		find[i++] = input[pos];
	return (find);
}

char	ft_strcmp_beg(char *str1, char *str2)
{
	int i;

	i = 0;
	while (str1[i] && str2[i] && str1[i] == str2[i])
		++i;
	if (str1[i] && str2[i])
		return (0);
	return (1);
}

void	startfind(char **ac, t_env *env, int boolean, int i)
{
	char	*tmp;
	char	*tmp2;

	while (ac[i])
	{
		tmp = to_lwcase(ac[i]);
		tmp2 = to_lwcase(env->find);
		if (!ft_strcmp_beg(tmp, tmp2))
		{
			free(ac[i]);
			boolean = i;
			while (ac[boolean++])
				ac[boolean - 1] = ac[boolean];
		}
		else
			++i;
		free(tmp);
		free(tmp2);
	}
}

char	**forest(char *s, int ps, t_env *env, char first)
{
	char	**ac;

	if (bs_str(s, ps, ' ') || s[ps] == '\0' || bs_str(s, ps + 1, ' ') || s[ps + 1] == '\0')
	{
		if (first)
			ac = auto_possibilities(0, env);
		else
			ac = auto_possibilities(1, env);
		if (s[ps - 1] != ' ' && env->find[0] != '/')
			startfind(ac, env, 1, 0);
	}
	else
	{
		if (first)
			ac = auto_possibilities(0, env);
		else
			ac = auto_possibilities(1, env);
		if (env->find[0] != '/')
			startfind(ac, env, 0, 0);
	}
	return (ac);
}

char	**autocomplete(char *input, int pos, t_env *env)
{
	char	first;
	int		i;
	char	**ac;
	char	quote;

	quote = 0;
	i = 0;
	while (input[i] && (bs_str(input, i, ' ') || bs_str(input, i, '\t')))
		++i;
	if (i > pos || input[i] == '\0')
		return (NULL);
	while (input[i] && (!bs_str(input, i, ' ') || quote != 0))
	{
		if (bs_str(input, i, '\'') || bs_str(input, i, '"'))
			quote = input[i] == quote ? 0 : quote;
		++i;
	}
	first = pos > i ? 0 : 1;
	env->find = finder(input, bs_str(input, pos, ' ') ? pos - 1 : pos);
	ft_tilde(&env->find, -1, 0);
	ac = forest(input, pos, env, first);
	free(env->find);
	env->find = NULL;
	return (ac);
}
