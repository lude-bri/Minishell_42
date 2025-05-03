/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   702_redirs_2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 02:37:16 by mde-agui          #+#    #+#             */
/*   Updated: 2024/12/23 02:44:30 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Executes the appropriate redirection based on the token type.
 *
 * Dispatches the redirection to the correct function: `redir_in`, `redir_out`, or `redir_append`.
 * For append operations, it additionally flags the following argument token as a redirection argument.
 *
 * @param tokens The current redirection token.
 * @param msh The minishell context structure.
 */
void	process_redirection(t_tkn *tokens, t_msh *msh)
{
	if (tokens->type == TKN_IN)
		redir_in(tokens, msh);
	else if (tokens->type == TKN_OUT)
		redir_out(tokens, msh);
	else if (tokens->type == TKN_APPEND)
	{
		redir_append(tokens, msh);
		if (tokens->next && tokens->next->next)
			tokens->next->next->type = TKN_REDIR_ARG;
	}
}

/**
 * @brief Updates the type of the next token to indicate it belongs to a redirection.
 *
 * Changes the token type to `TKN_REDIR_ARG`, unless the next token is a pipe.
 *
 * @param tokens The current redirection token.
 */
void	update_token_types(t_tkn *tokens)
{
	if (tokens->next != NULL)
	{
		if (tokens->next->type == TKN_PIPE)
			return ;
		else
			tokens->next->type = TKN_REDIR_ARG;
	}
}

/**
 * @brief Iterates through tokens and processes all redirections before the next pipe.
 *
 * Handles input, output, and append redirections by calling `process_redirection()` and updating token types.
 *
 * @param tokens The list of tokens to scan.
 * @param msh The minishell context structure.
 */
void	handle_redirs(t_tkn *tokens, t_msh *msh)
{
	while (tokens)
	{
		if (tokens->type == TKN_PIPE)
			break ;
		if (tokens->type == TKN_IN || tokens->type == TKN_OUT
			|| tokens->type == TKN_APPEND)
		{
			process_redirection(tokens, msh);
			update_token_types(tokens);
		}
		tokens = tokens->next;
	}
}

/**
 * @brief Reconstructs the command argument array (`cmds->av`) from TKN_CMD tokens.
 *
 * Collects command arguments up to the next pipe and builds a new argument list.
 * Marks `msh->flag_redir` to signal that redirection affected the command.
 *
 * @param tokens The list of tokens to scan.
 * @param msh The minishell context structure.
 */
void	handle_command_arguments(t_tkn *tokens, t_msh *msh)
{
	int	i;

	i = 0;
	msh->arg = ft_calloc(1024 + 1, sizeof(char *));
	while (tokens)
	{
		if (tokens->type == TKN_PIPE)
			break ;
		if (tokens->type == TKN_CMD && tokens->type != TKN_REDIR_ARG)
		{
			msh->arg[i] = ft_strdup(tokens->name);
			i++;
		}
		tokens = tokens->next;
	}
	msh->arg[i] = NULL;
	free_arg(msh->cmds->av);
	msh->cmds->av = msh->arg;
	msh->flag_redir = true;
}

/**
 * @brief Main entry point to apply redirections and rebuild command arguments.
 *
 * Combines `handle_redirs()` and `handle_command_arguments()` to:
 * 1. Execute redirections (`<`, `>`, `>>`)
 * 2. Update `cmds->av` to reflect only the actual command and its arguments.
 *
 * @param tokens The list of tokens to process.
 * @param msh The minishell context structure.
 */
void	redirs_2(t_tkn *tokens, t_msh *msh)
{
	handle_redirs(tokens, msh);
	handle_command_arguments(tokens, msh);
}
