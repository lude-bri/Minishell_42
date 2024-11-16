/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   400_signals.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 19:15:51 by mde-agui          #+#    #+#             */
/*   Updated: 2024/10/24 23:49:55 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	sigquit_handler(int sig)
{
	(void)sig;
}

void	sigint_handler(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	//Prepares readline to receive a new prompt 
	rl_on_new_line();
	//Clears the input
	// rl_replace_line("", 0);
	//Redisplays the prompt
	rl_redisplay();
}

void	setup_signals(void)
{
	struct sigaction	sa_int;
	// struct sigaction	sa_quit;

	//SIGINT (ctrl-c)
	sa_int.sa_handler = sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);

	//SIGQUIT (ctrl-\)(It does nothing)
	signal(SIGQUIT, SIG_IGN);
	/*sa_quit.sa_handler = sigquit_handler;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = SA_RESTART;
	sigaction(SIGQUIT, &sa_quit, NULL);*/
}
