/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   420_exec_one.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:40:02 by luigi             #+#    #+#             */
/*   Updated: 2025/01/02 16:13:30 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_bi(t_tkn *tokens)
{
	if (tokens->cmd_type == CMD_CD)
		return (SUCCESS);
	if (tokens->cmd_type == CMD_PWD)
		return (SUCCESS);
	if (tokens->cmd_type == CMD_ENV)
		return (SUCCESS);
	if (tokens->cmd_type == CMD_ECHO)
		return (SUCCESS);
	if (tokens->cmd_type == CMD_EXIT)
		return (SUCCESS);
	if (tokens->cmd_type == CMD_UNSET)
		return (SUCCESS);
	if (tokens->cmd_type == CMD_EXPORT)
		return (SUCCESS);
	else
		return (FAILURE);
}

static int	is_special_bi(t_tkn *tokens)
{
	if (tokens->cmd_type == CMD_CD)
		return (SUCCESS);
	if (tokens->cmd_type == CMD_PWD)
		return (SUCCESS);
	if (tokens->cmd_type == CMD_ENV)
		return (SUCCESS);
	if (tokens->cmd_type == CMD_EXIT)
		return (SUCCESS);
	if (tokens->cmd_type == CMD_UNSET)
		return (SUCCESS);
	if (tokens->cmd_type == CMD_EXPORT)
		return (SUCCESS);
	else
		return (FAILURE);
}

void	handle_builtin_commands(t_msh *msh, t_tkn *tokens)
{
	int	pid;
	int	status;

	if (is_special_bi(tokens) == SUCCESS)
		exec_bi(tokens, msh);
	else
	{
		pid = fork();
		if (pid == 0)
		{
			exec_bi(tokens, msh);
			free_arg(msh->envp);
			free_msh(msh->cmds, msh, tokens);
			exit(msh->exit_status);
		}
		else if (pid > 0)
		{
			waitpid(pid, &status, 0);
			if (WIFEXITED(status))
				msh->exit_status = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				msh->exit_status = 128 + WTERMSIG(status);
		}
	}
}

void	exec_one_redirs(t_msh *msh, t_tkn *tokens)
{
	// if (tokens->type == TKN_HEREDOC && tokens->next != NULL)
	// 	heredoc(tokens, msh, tokens->next->name, 1);
	// else if (tokens->next != NULL)
	exec_exe(tokens, msh);
}

int	exec_one(t_msh *msh, t_tkn *tokens)
{
	while (tokens && tokens->type == TKN_NULL)
		tokens = tokens->next;
	if (!tokens)
		return (SUCCESS);
	if (tokens->type == TKN_CMD)
	{
		if (is_bi(tokens) == SUCCESS)
			handle_builtin_commands(msh, tokens);
		else
			exec_exe(tokens, msh);
	}
	else if (tokens->type == TKN_IN || tokens->type == TKN_OUT
		|| tokens->type == TKN_APPEND || tokens->type == TKN_HEREDOC)
		exec_one_redirs(msh, tokens);
	return (SUCCESS);
}
