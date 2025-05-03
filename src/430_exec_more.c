/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   430_exec_more.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:57:51 by luigi             #+#    #+#             */
/*   Updated: 2024/11/14 10:42:42 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Executes a command tree with one or more piped commands.
 *
 * - If the root token is a pipe (`TKN_PIPE`), ensures the syntax is valid
 *   and delegates to `exec_pipe()` for recursive pipe execution.
 * - If the token is a single non-piped command:
 *   - Executes builtin commands directly via `exec_bi()`
 *   - Executes external commands via `exec_exe()`
 *
 * This function operates on a binary tree built by `create_bin_tree()`.
 *
 * @param msh Pointer to the shell state.
 * @param tokens Pointer to the root of the command tree.
 * @return SUCCESS if all commands execute successfully; FAILURE otherwise.
 */
int	exec_more(t_msh *msh, t_tkn *tokens)
{
	if (tokens->type == TKN_PIPE)
	{
		if (ft_strcmp(tokens->name, "|") != 0)
		{
			syntax_check(msh, tokens);
			return (SUCCESS);
		}
		if (exec_pipe(msh, tokens) != SUCCESS)
			return (FAILURE);
	}
	else
	{
		if (is_bi(tokens) == SUCCESS)
		{
			exec_bi(tokens, msh);
			return (SUCCESS);
		}
		if (exec_exe(tokens, msh) != SUCCESS)
			return (FAILURE);
	}
	return (SUCCESS);
}
