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

/**
 * @brief Checks if a token corresponds to a valid builtin command.
 *
 * Recognized builtins:
 * - `cd`
 * - `pwd`
 * - `env`
 * - `echo`
 * - `exit`
 * - `unset`
 * - `export`
 *
 * @param tokens Pointer to the command token.
 * @return SUCCESS if it's a builtin; FAILURE otherwise.
 */
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

/**
 * @brief Checks if a builtin should be executed **without** forking.
 *
 * These are builtins that affect shell state directly, such as:
 * - `cd`, `export`, `unset`, `exit`, `env`, `pwd`
 * They must run in the parent process to persist their changes.
 *
 * @param tokens Pointer to the command token.
 * @return SUCCESS if special builtin; FAILURE otherwise.
 */
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

/**
 * @brief Handles the execution of a builtin command.
 *
 * - If it's a special builtin (e.g. `cd`), executes directly in parent.
 * - Otherwise, forks and runs in a child process.
 *
 * Signal handlers are reset to default before forking.
 *
 * @param msh Pointer to the shell state.
 * @param tokens Pointer to the command token.
 */
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

/**
 * @brief Checks if the token list contains only null tokens.
 *
 * This function is used to detect cases like empty input or invalid
 * tokens that the parser didn't classify as commands.
 *
 * @param tokens Pointer to the token list.
 * @return 1 if only null tokens are present; 0 otherwise.
 */
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

/**
 * @brief Executes a single command (non-piped).
 *
 * - Skips null tokens and checks for invalid input.
 * - Executes builtins with or without forking as needed.
 * - Executes external commands using `exec_exe()`.
 * - Handles redirections like `<`, `>`, `>>`.
 *
 * @param msh Pointer to the shell state.
 * @param tokens Pointer to the full token list.
 * @return Always returns SUCCESS (errors handled internally).
 */
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
