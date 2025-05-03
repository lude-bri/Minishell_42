/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   410_signals.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 19:15:51 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/11 15:47:27 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Dummy handler for SIGQUIT (Ctrl+\).
 *
 * This function does nothing but overrides the default behavior,
 * effectively ignoring `SIGQUIT` in the shell.
 *
 * @param sig The signal number (unused).
 */
void	sigquit_handler(int sig)
{
	(void)sig;
}

/**
 * @brief Handler for SIGINT (Ctrl+C) during user input.
 *
 * If no child process is running (`waitpid(-1, NULL, WNOHANG) == -1`),
 * this means the shell is idle and waiting for user input:
 * - A newline is printed
 * - The current line is cleared and redisplayed via Readline
 *
 * Also sets the global signal flag `g_signal = 130`, which may be used
 * elsewhere to track signal-triggered interrupts.
 *
 * @param sig The signal number (unused).
 */
void	sigint_handler(int sig)
{
	(void)sig;
	if (waitpid(-1, NULL, WNOHANG) == -1)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	g_signal = 130;
}

/**
 * @brief Sets up signal handling for the shell prompt.
 *
 * - SIGINT is caught and handled by `sigint_handler`
 * - SIGQUIT is ignored (`SIG_IGN`)
 * - Uses `sigaction` for reliable handling with `SA_RESTART` to ensure
 *   system calls are restarted after signal handling.
 */
void	setup_signals(void)
{
	struct sigaction	sa_int;

	sa_int.sa_handler = sigint_handler;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	signal(SIGQUIT, SIG_IGN);
}
