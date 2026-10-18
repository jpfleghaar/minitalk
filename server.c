/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 13:41:34 by jpflegha          #+#    #+#             */
/*   Updated: 2025/02/25 14:35:37 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

char	*handle_len(int signo, int *phase, pid_t client_pid)
{	
	static char		*str;
	static size_t	len;
	static int		i;

	len = (len << 1) | (signo == SIGUSR1 ? 1 : 0);
	i++;
	if (32 == i)
	{
		str	= malloc(sizeof(char) * (len + 1));
		if (!str)
		{
			write(STDOUT_FILENO, "Malloc failed\n", 14);
			exit(EXIT_FAILURE);
		}
		i = 0;
		*phase = 1;
		str[len] = '\0';
		len = 0;
		return (str);
	}
	ft_kill(client_pid, SIGUSR1);
	return (NULL);
		
}
void	handler_messag(int signo, char *str, pid_t client_pid, int *phase)
{
	static char		c;
	static int		i;
	static size_t   len;
	
	c = (c << 1) | (signo == SIGUSR1 ? 1 : 0);
	i++;
	if (8 == i)
	{
		i = 0;
		str[len] = c;
		if ('\0' == c)
		{
			write(STDOUT_FILENO, str, len);
			write(STDOUT_FILENO, "\n", 1);
			free(str);
			len = 0;
			*phase = 0;
			kill(client_pid, SIGUSR2);
			client_pid = 0;
			return ;
		}
		len++;
		c = 0;
	}
	ft_kill(client_pid, SIGUSR1);
}

void handler(int signo, siginfo_t *info, void *more_info)
{
	static pid_t	client_pid;
	static int		phase;
	static char		*str;

	(void)more_info;
	client_pid = info->si_pid;
	if(phase == 0)
	{
		str = handle_len(signo,&phase, client_pid);
		if(str)
			ft_kill(client_pid, SIGUSR1);
		return ;
	}
	else
		handler_messag(signo, str, client_pid, &phase);
}

int	main(void)
{
	int pid;

	pid = getpid();
	write(STDOUT_FILENO, "PID: ", 5);
	ft_putpid(pid);
	write(STDOUT_FILENO, "\n", 1);
	my_signal(SIGUSR1, handler, true);
	my_signal(SIGUSR2, handler, true);
	
	while (1)
	{
		pause();
	}
	return (EXIT_SUCCESS);
}
