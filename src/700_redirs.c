/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   700_redirs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 09:09:40 by luigi             #+#    #+#             */
/*   Updated: 2025/01/02 16:14:12 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Debug helper that checks for '-' flags in the upcoming token.
 *
 * This function is used for diagnostic output to determine whether
 * the token after a redirection might contain execution flags (e.g., `-n`, `-l`).
 *
 * @param tokens The current redirection token.
 */
static void	flags_check(t_tkn *tokens)
{
	if (ft_strchr(tokens->next->next->name, '-') != 0)
		printf("and it is exec\n");
	else
		printf("and it is not exec\n");
}

/**
 * @brief Handles redirection based on the token type.
 *
 * Dispatches the token to the correct redirection handler:
 * - `redir_in()` for input (`<`)
 * - `redir_out()` for output (`>`)
 * - `redir_append()` for append (`>>`)
 *
 * Also includes a debug check to determine if there is a following command token.
 *
 * @param tokens The redirection token to process.
 * @param msh The minishell context structure.
 */
void	redirs(t_tkn *tokens, t_msh *msh)
{
	t_tkn	*tkn_exec;

	tkn_exec = tokens;
	if (tokens->next->next != NULL && tokens->next->next->type != TKN_PIPE)
	{
		printf("there is something\n");
		flags_check(tokens);
	}
	if (tokens->type == TKN_IN)
		redir_in(tkn_exec, msh);
	else if (tokens->type == TKN_OUT)
		redir_out(tkn_exec, msh);
	else if (tokens->type == TKN_APPEND)
		redir_append(tkn_exec, msh);
}

/**
 * @brief Iterates through tokens and applies redirections when found.
 *
 * This function walks through all tokens and, when a redirection token is
 * encountered (`<`, `>`, or `>>`), it calls `redirs_2()` to apply it.
 *
 * @param tokens The list of tokens to scan.
 * @param msh The minishell context structure.
 * @return Always returns SUCCESS (0).
 */
int	exec_redirs(t_tkn *tokens, t_msh *msh)
{
	t_tkn	*tkn;

	tkn = tokens;
	while (tkn)
	{
		if (tkn->type == TKN_IN || tkn->type == TKN_OUT
			|| tkn->type == TKN_APPEND)
		{
			redirs_2(tokens, msh);
		}
		tkn = tkn->next;
	}
	return (SUCCESS);
}
