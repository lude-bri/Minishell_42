/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   403_execute_misc2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 17:09:35 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/12 11:42:52 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_child_process(t_tkn *tokens, t_msh *msh)
{
	exec_redirs(tokens, msh);
	execute(msh, tokens);
}

void	handle_parent_process(t_msh *msh, int status)
{
	setup_signals();
	waitpid(msh->pid, &status, 0);
	if (WIFEXITED(status))
		msh->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		msh->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
	}
}

void	handle_command_not_found(t_msh *msh, t_tkn *tokens, char **args)
{
	write(STDERR_FILENO, " command not found\n", 19);
	free_msh(msh->cmds, msh, tokens);
	free_array(msh->envp, 0);
	free_arg(msh->ex_envp);
	free_arg(args);
	exit(127);
}

void	handle_execve_error(char *path, char **args, t_msh *msh, t_tkn *tokens)
{
	perror(path);
	free(path);
	free_arg(args);
	free_msh(msh->cmds, msh, tokens);
	free_array(msh->envp, 0);
	free_arg(msh->ex_envp);
	exit(126);
}
