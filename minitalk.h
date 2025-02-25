/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jpflegha <jpflegha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 15:02:18 by jpflegha          #+#    #+#             */
/*   Updated: 2025/02/25 18:07:49 by jpflegha         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

enum
{
	READY,
	BUSY,
};

void	end_handler(void);

void	ack_handler(void);

void	send_len(pid_t server, size_t len);

void	send_message(pid_t server, char c);

void	handler(int signo, siginfo_t *info, void *more_info);

char	*handle_len(int signo, int *phase, pid_t client_pid);

int		handler_messag(int signo, char *str, pid_t client_pid, int *phase);

void	my_signal(int signo, void *handler, bool use_sigi);

void	ft_kill(pid_t pid, int signum);

size_t	ft_strlen(const char *s);

void	ft_putpid(int n);
#endif