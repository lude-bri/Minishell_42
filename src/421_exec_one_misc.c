/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   421_exec_one_misc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 11:48:29 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/12 12:01:33 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_child_builtin(t_msh *msh, t_tkn *tokens)
{
	exec_bi(tokens, msh);
	free_and_close_all(msh, tokens);
	exit(msh->exit_status);
}

void	handle_parent_builtin(int pid, t_msh *msh)
{
	int	status;

	setup_signals();
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		msh->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		msh->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
	}
}
