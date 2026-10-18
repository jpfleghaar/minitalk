/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 13:40:53 by jpflegha          #+#    #+#             */
/*   Updated: 2025/02/25 14:38:05 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

volatile sig_atomic_t	g_server_status = BUSY;

void	end_handler(void)
{
	write(STDOUT_FILENO, "Message received\n", 17);
	exit(EXIT_SUCCESS);
}

void	ack_handler(void)
{
	g_server_status = READY;
}

void	send_len(pid_t server, size_t len)
{
	int	i;

	i = 31;
	while (i >= 0)
	{
		if (len & (1 << i))
			ft_kill(server, SIGUSR1);
		else
			ft_kill(server, SIGUSR2);
		while (BUSY == g_server_status)
		{
			usleep(50);
		}
		g_server_status = BUSY;
		i--;
	}
}

void	send_message(pid_t server, char c)
{
	int	i;

	i = 7;
	while (i >= 0)
	{
		if (c & (1 << i))
		{
			ft_kill(server, SIGUSR1);
		}
		else
		{
			ft_kill(server, SIGUSR2);
		}
		while (BUSY == g_server_status)
			usleep(50);
		g_server_status = BUSY;
		i--;
	}
}

int	main(int ac, char **av)
{
	pid_t	server;
	char	*str;
	int		i;
	size_t	len;

	i = 0;
	if (3 != ac || atoi(av[1]) <= 0 || !av[2])
	{
		write(STDOUT_FILENO, "Usage: [server_pid] [message]\n", 30);
		return (EXIT_FAILURE);
	}
	server = atoi(av[1]);
	str = av[2];
	len = ft_strlen(str);
	my_signal(SIGUSR1, ack_handler, false);
	my_signal(SIGUSR2, end_handler, false);
	send_len(server, len);
	while (str[i])
	{
		send_message(server, str[i]);
		i++;
	}
	send_message(server, '\0');
	return (EXIT_SUCCESS);
}
