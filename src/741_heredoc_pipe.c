/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   741_heredoc_pipe.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 03:02:43 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/02 16:11:58 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	heredoc_pipe(t_tkn *tokens, t_msh *msh, char *arg, int flag)
{
	int	pipe_fd[2];

	// setup_pipe_and_signals(tokens, msh, pipe_fd);
	// read_n_write_lines(tokens, msh, arg, pipe_fd);
	(void)tokens;
	(void)arg;
	if (flag == 0)
		dup2(pipe_fd[0], STDIN_FILENO);
	signal(SIGINT, SIG_DFL);
	free_arg(msh->envp);
	free_arg(msh->ex_envp);
	return (pipe_fd[0]);
}
