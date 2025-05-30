/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   100_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:03:56 by luigi             #+#    #+#             */
/*   Updated: 2025/01/07 19:43:01 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

volatile int	g_signal = 0;

/**
 * @brief Entry point of the minishell program.
 *
 * Initializes the main shell structure, sets up the environment,
 * and starts the shell loop.
 *
 * @param ac Argument count (unused).
 * @param av Argument vector (unused).
 * @param envp Environment variables.
 * @return Exit status of the shell program.
 */
int	main(int ac, char **av, char **envp)
{
	t_msh		msh;

	(void)ac;
	(void)av;
	ft_memset(&msh, 0, sizeof(t_msh));
	init_msh(&msh, envp);
	msh_loop(&msh);
	return (free_array(msh.envp, 0));
}

/**
 * @brief Entry point of the minishell program.
 *
 * Initializes the main shell structure, sets up the environment,
 * and starts the shell loop.
 *
 * @param ac Argument count (unused).
 * @param av Argument vector (unused).
 * @param envp Environment variables.
 * @param tokens Transforms arguments in tokens and then executes
 * @return Exit status of the shell program.
 */
int	msh_loop(t_msh *msh)
{
	t_tkn	*tokens;

	tokens = NULL;
	while (1)
	{
		setup_signals();
		init_struct(msh);
		tokens = to_parse(msh);
		if (!tokens)
		{
			if (msh->heredoc)
				free(msh->heredoc);
			continue ;
		}
		if (msh->cmd_count > NO_CMDS)
			if (!to_execute(msh, tokens))
				break ;
		free_msh(msh->cmds, msh, tokens);
	}
	return (0);
}
