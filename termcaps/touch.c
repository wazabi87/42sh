/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   touch.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleclair <mleclair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/18 14:32:07 by mleclair          #+#    #+#             */
/*   Updated: 2017/05/10 17:20:01 by mleclair         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../chell.h"

void	touch4(t_var *var)
{
	if (var->buff[0] == 18 && var->buff[1] == 0 && var->buff[2] == 0)
		control_r(var, ft_strdup(var->ret), NULL, var->lenprompt);
	if (var->buff[0] == 27 && var->buff[2] == 70)
		end(var);
	if (var->del == 1)
		var->del = 0;
	if (var->buff[0] == 127 && var->buff[1] == 0)
		backspace(var);
	if (var->buff[0] == 27 && var->buff[2] == 51)
	{
		var->del = 1;
		deleteu(var);
	}
}

void	touch3(t_var *var, int *i, int *bg)
{
	if (var->buff[0] == 9 && var->buff[2] == 0)
		tabu(var, i);
	else if (var->buff[0] == 27 && var->buff[2] == 65 && env()->history[0])
		up_arrow(var, bg);
	else if (var->buff[0] == 27 && var->buff[2] == 66 && var->his)
		down_arrow(var, bg);
	else
	{
		free(var->arr);
		var->arr = NULL;
		while (env()->history[*bg])
			++(*bg);
		free_double_array(var->ac);
		free_double_array(var->his);
		var->ac = NULL;
		var->his = NULL;
		*i = 0;
	}
}

void	touch2(t_var *var)
{
	if (var->buff[0] == -30 && var->buff[1] == -120)
		paste(var);
	if (var->buff[0] == 27 && var->buff[2] == 68 && var->i > 0)
		left_arrow(var);
	if (var->buff[0] == 27 && var->buff[2] == 67)
		right_arrow(var);
	if (var->buff[0] == 59 && var->buff[2] == 68)
		shift_arrow_l(var);
	if (var->buff[0] == 59 && var->buff[2] == 67)
		shift_arrow_r(var);
	if (var->buff[0] == 59 && var->buff[2] == 65)
		shift_up(var);
	if (var->buff[0] == 59 && var->buff[2] == 66)
		shift_down(var);
	if (var->buff[0] == 27 && var->buff[2] == 72)
		home(var);
}

void	touch1(t_var *var, int *bg, int *i)
{
	read(0, var->buff, 3);
	if (var->buff[0] == 27 && var->buff[2] == 91)
		select_mode(var);
	else if (var->buff[1] == -119)
		cut(var);
	else if (var->buff[1] == -89)
		copy(var, 0);
	else if (var->selmode == 1 && var->buff[0] > 0)
		desel(var);
	if (var->buff[0] == 3 && var->buff[1] == 0)
		return ;
	touch2(var);
	touch3(var, i, bg);
	touch4(var);
	touch5(var);
}

void	touch(t_var *var, t_ssprintf *prompt)
{
	int i;

	i = 0;
	var->bg = 0;
	while (env()->history[var->bg])
		++var->bg;
	var->i = 0;
	ft_putstr_fd(tgetstr("im", NULL), env()->fdout);
	ft_putstr_fd(tgetstr("bw", NULL), env()->fdout);
	while (var->buff[0] != 10)
		touch1(var, &var->bg, &i);
	while (var->i != (int)ft_strlen(var->ret))
		right_arrow(var);
	ft_putstr_fd(tgetstr("cd", NULL), env()->fdout);
	while (exclam(var, -1))
		;
	if (var->ret[0] && ft_strcmp("\nhist-i-search : ", prompt->buf))
		add_history(var->ret);
	add_bs_q(&var->ret, -1, '\'', 0);
	write(env()->fdout, "\n", 1);
	ft_putstr_fd(tgetstr("ei", NULL), env()->fdout);
}
