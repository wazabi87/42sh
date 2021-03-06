/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aperture.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mleclair <mleclair@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/01/29 16:36:54 by mleclair          #+#    #+#             */
/*   Updated: 2017/03/22 19:32:42 by aridolfi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "chell.h"

void	aperture3(int truc, char *buf, int *fd, char *tmp)
{
	char	*lol[3];

	if (truc == 0)
	{
		lol[0] = "afplay";
		lol[1] = "stillalive.mp3";
		lol[2] = NULL;
		execve("/usr/bin/afplay", lol, env()->ev);
	}
	while (read(fd[0], buf, 1))
	{
		write(1, &buf[0], 1);
		if (*buf != '\n')
		{
			get_next_line(fd[1], &tmp);
			usleep(ft_atoi(tmp) + 1000);
			free(tmp);
		}
	}
}

void	aperture2(void)
{
	char	buf[5];
	char	*tmp;
	int		fd[2];
	int		truc;

	truc = fork();
	env()->boolthing = truc;
	if (truc == 0)
	{
		env()->i = 0;
		if ((fd[0] = open("stillalive.txt", O_RDONLY)) == -1)
			return (ft_putstr("But why delete it ? you monster\n"));
		if ((fd[1] = open("stillalivetimer.txt", O_RDONLY)) == -1)
			return (ft_putstr("But why delete it ? you monster\n"));
		truc = fork();
		tmp = NULL;
		aperture3(truc, buf, fd, tmp);
		ft_putstr("\033[0m");
		exit(0);
	}
	else
		wait(NULL);
}

void	ft_aperture(void)
{
	ft_putstr(tgetstr("cl", NULL));
	ft_printf("\e[38;5;179m");
	ft_printf("%s\n", "");
	ft_printf("%s\n", "              .,-:;//;:=,");
	ft_printf("%s\n", "          . :H@@@MM@M#H/.,+%;,");
	ft_printf("%s\n", "       ,/X+ +M@@M@MM%=,-%HMMM@X/,");
	ft_printf("%s\n", "     -+@MM; $M@@MH+-,;XMMMM@MMMM@+-");
	ft_printf("%s\n", "    ;@M@@M- XM@X;. -+XXXXXHHH@M@M#@/.");
	ft_printf("%s\n", "  ,%MM@@MH ,@%=             .---=-=:=,.");
	ft_printf("%s\n", "  =@#@@@MX.,                -%HX$$%%%:;");
	ft_printf("%s\n", " =-./@M@M$                   .;@MMMM@MM:");
	ft_printf("%s\n", " X@/ -$MM/                    . +MM@@@M$");
	ft_printf("%s\n", ",@M@H: :@:                    . =X#@@@@-");
	ft_printf("%s\n", ",@@@MMX, .                    /H- ;@M@M=");
	ft_printf("%s\n", ".H@@@@M@+,                    %MM+..%#$.");
	ft_printf("%s\n", " /MMMM@MMH/.                  XM@MH; =;");
	ft_printf("%s\n", "  /%+%$XHH@$=              , .H@@@@MX,");
	ft_printf("%s\n", "   .=--------.           -%H.,@@@@@MX,");
	ft_printf("%s\n", "   .%MM@@@HHHXX$$$%+- .:$MMX =M@@MM%.");
	ft_printf("%s\n", "     =XMMM@MM@MM#H;,-+HMM@M+ /MMMX=");
	ft_printf("%s\n", "       =%@M@M#@$-.=$@MM@@@M; %M%=");
	ft_printf("%s\n", "         ,:+$+-,/H#MMMMMMM@= =,");
	ft_printf("%s\n", "               =++%%%%+/:-.");
	aperture2();
}
