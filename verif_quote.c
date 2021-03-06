/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   verif_quote.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleclair <mleclair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/31 17:51:24 by mleclair          #+#    #+#             */
/*   Updated: 2017/04/28 18:19:33 by mleclair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chell.h"

void	add_bs_q(char **str, int i, char c, int l)
{
	int		j;
	int		k;
	char	*tmp;

	while ((*str)[++i])
		if (bs_str(*str, i, c) && add_bs_q2(str, c, i + 1) == 1)
		{
			k = 0;
			j = 0;
			tmp = palloc(ft_strlen(*str) * 2 + 1);
			tmp[j++] = (*str)[i];
			(*str)[i] = 0;
			l = i;
			while (!bs_str(*str, ++i, c) && (*str)[i])
			{
				tmp[j++] = '\\';
				tmp[j++] = (*str)[i];
			}
			k = j;
			while ((*str)[i])
				tmp[j++] = (*str)[i++];
			tmp[j] = 0;
			*str = ft_strjoinfree(*str, tmp, 3);
			i = k + l;
		}
}

int		ft_strfind_bq(const char *s, int c)
{
	int		i;

	i = 0;
	if (!s)
		return (-1);
	while (s[i] && (s[i] != c || (i > 0 && s[i - 1] == '\\')))
		i++;
	return (s[i] == c ? i : -1);
}

int		complete_dquote(char **str, int pos)
{
	char	*tmp;
	char	*tmp3;

	tmp = ft_strdup(*str);
	while (ft_strfind_bq(tmp + pos + 1, '"') == -1 && env()->bool1 == 0)
	{
		tmp3 = termcaps(ft_sprintf("dquote>"), 7, 0);
		if (env()->bool1 == 1)
		{
			env()->bool2 = 1;
			free(*str);
			free(tmp);
			*str = tmp3;
			return (0);
		}
		tmp = ft_strjoinfree(tmp, "\n", 1);
		tmp = ft_strjoinfree(tmp, tmp3, 3);
	}
	free(*str);
	*str = tmp;
	if (ver_dquote_t2(*str) == -1)
		return (-1);
	return (0);
}

int		complete_quote(char **str, int pos)
{
	char *tmp;
	char *tmp3;

	tmp = ft_strdup(*str);
	while (ft_strfind_bq(tmp + pos + 1, '\'') == -1 && env()->bool1 == 0)
	{
		tmp3 = termcaps(ft_sprintf("quote>"), 6, 0);
		if (env()->bool1 == 1)
		{
			env()->bool2 = 1;
			free(*str);
			free(tmp);
			*str = tmp3;
			return (-1);
		}
		tmp = ft_strjoinfree(tmp, "\n", 1);
		tmp = ft_strjoinfree(tmp, tmp3, 3);
	}
	free(*str);
	*str = tmp;
	return (1);
}

int		verif_quote(char **str, int p, int quote, int dquote)
{
	int pos;

	dquote = 0;
	pos = 0;
	while ((*str)[++p])
	{
		if ((bs_str((*str), p, '\'') || bs_str((*str), p, '"')) && quote == 0
			&& dquote == 0)
			pos = p;
		if (bs_str((*str), p, '\'') && dquote == 0)
			quote = quote == 1 ? 0 : 1;
		if (bs_str((*str), p, '"') && quote == 0)
			dquote = dquote == 1 ? 0 : 1;
	}
	if (quote == 1)
		return (complete_quote(str, pos));
	else if (dquote == 1)
	{
		if (complete_dquote(str, pos))
			return (-1);
	}
	else
		return (0);
	return (1);
}
