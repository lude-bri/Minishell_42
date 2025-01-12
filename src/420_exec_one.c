/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   420_exec_one.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:40:02 by luigi             #+#    #+#             */
/*   Updated: 2025/01/12 11:52:52 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_bi(t_tkn *tokens)
{
	if (tokens->cmd_type)
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
	int					pid;
	struct sigaction	sa_default;

	if (is_special_bi(tokens) == SUCCESS)
		exec_bi(tokens, msh);
	else
	{
		sa_default.sa_handler = SIG_DFL;
		sigemptyset(&sa_default.sa_mask);
		sa_default.sa_flags = 0;
		sigaction(SIGINT, &sa_default, NULL);
		sigaction(SIGQUIT, &sa_default, NULL);
		pid = fork();
		if (pid == 0)
			handle_child_builtin(msh, tokens);
		else if (pid > 0)
			handle_parent_builtin(pid, msh);
	}
}

static int	verify_only_null(t_tkn *tokens)
{
	t_tkn	*tkn;
	int		flag;
	int		len;

	tkn = tokens;
	len = tokens->len;
	while (tkn && len--)
	{
		if (tkn->type == TKN_NULL)
			flag = 1;
		else
		{
			flag = 0;
			return (flag);
		}
		tkn = tkn->next;
	}
	return (flag);
}

int	exec_one(t_msh *msh, t_tkn *tokens)
{
	if (verify_only_null(tokens) == 1)
	{
		write(2, " command not found\n", 19);
		msh->exit_status = 127;
	}
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
		|| tokens->type == TKN_APPEND)
		exec_exe(tokens, msh);
	return (SUCCESS);
}
