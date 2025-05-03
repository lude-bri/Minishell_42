/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   120_prompt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 12:23:10 by luigi             #+#    #+#             */
/*   Updated: 2025/01/07 19:41:43 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Displays the shell prompt and reads user input.
 *
 * Uses `readline()` to display a coloured prompt and capture user input.
 * Handles special cases like Ctrl+C interruption (signal 130) and EOF (Ctrl+D).
 *
 * - If the user sends an interrupt (SIGINT), sets exit status to 130.
 * - If the input is empty (just ENTER), returns NULL.
 * - If `readline` returns NULL (EOF), prints "exit" and safely frees memory before exiting.
 *
 * @param msh Pointer to the shell state structure (t_msh), used to update exit status and free resources on EOF.
 * @return The user input as a newly allocated string, or NULL if input is empty.
 */
char	*build_prompt(t_msh *msh)
{
	char	*line;

	line = readline("\001\033[1;32m\002msh$ \001\033[0m\002");
	if (g_signal == 130)
	{
		msh->exit_status = 130;
		g_signal = 0;
	}
	if (!line)
	{
		printf("exit\n");
		free_arg(msh->envp);
		free_arg(msh->ex_envp);
		free(msh->heredoc);
		exit (0);
	}
	else if (line[0] == '\0')
		return (NULL);
	return (line);
}
