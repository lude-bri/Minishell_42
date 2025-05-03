/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   320_syntax.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 11:27:09 by luigi             #+#    #+#             */
/*   Updated: 2024/12/22 23:28:27 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	find_redir(t_tkn *tokens);
static int	find_cmd(t_tkn *tokens);

/**
 * @brief Checks for syntax errors related to pipe tokens.
 *
 * Handles cases such as:
 * - A pipe at the beginning of the input
 * - Consecutive pipes (e.g., `||`)
 * - A pipe following an invalid token
 * - An excessive number of pipes (arbitrary limit of 7)
 *
 * If an error is detected, an appropriate error message is printed and
 * the shell exit status is set to 2.
 *
 * @param msh Pointer to the shell state.
 * @param tokens Pointer to the current token list.
 * @return FAILURE if a syntax error is found; otherwise SUCCESS.
 */
int	syntax_check_pipes(t_msh *msh, t_tkn *tokens)
{
	if (tokens->type == TKN_PIPE)
		printf("msh: syntax error near unexpected token `%s'\n", tokens->name);
	else if (tokens->type == TKN_PIPE && tokens->next->type == TKN_PIPE)
		printf("msh: syntax error near unexpected token `%s'\n", tokens->name);
	else if ((tokens->type == TKN_PIPE && tokens->next->type == TKN_PIPE
			&& tokens->next->type == TKN_PIPE) || msh->pipe_count > 7)
		printf("msh: syntax error near unexpected token `||'\n");
	else if (tokens->type != TKN_CMD && tokens->next->type == TKN_PIPE)
		printf("msh: syntax error near unexpected token `%s'\n", tokens->name);
	msh->exit_status = 2;
	return (FAILURE);
}

/**
 * @brief Checks for syntax errors involving redirection tokens.
 *
 * Ensures that redirection operators (`<`, `>`, `>>`, `<<`) are followed by
 * a valid command or file. If a redirection is the last token or no valid
 * command follows, a syntax error is printed.
 *
 * @param msh Pointer to the shell state.
 * @param tokens Pointer to the token list.
 * @return FAILURE if a syntax error is found; otherwise SUCCESS.
 */
int	syntax_check_redirs(t_msh *msh, t_tkn *tokens)
{
	if (find_cmd(tokens) == SUCCESS)
		return (SUCCESS);
	while (tokens)
	{
		if (tokens->type == TKN_IN || tokens->type == TKN_OUT
			|| tokens->type == TKN_APPEND || tokens->type == TKN_HEREDOC)
			break ;
		else
			tokens = tokens->next;
	}
	if (tokens->next == NULL || (find_redir(tokens) == SUCCESS
			&& find_cmd(tokens) == FAILURE))
	{
		printf("msh: syntax error near unexpected token `newline'\n");
		msh->exit_status = 2;
		return (FAILURE);
	}
	return (SUCCESS);
}

/**
 * @brief Helper function to check whether a command exists in the token list.
 *
 * Recognizes a valid command if a token of type TKN_CMD or a command
 * of type CMD_ECHO is found.
 *
 * @param tokens Pointer to the token list.
 * @return SUCCESS if a command is found; otherwise FAILURE.
 */
static int	find_cmd(t_tkn *tokens)
{
	while (tokens)
	{
		if (tokens->type == TKN_CMD || tokens->cmd_type == CMD_ECHO)
			return (SUCCESS);
		tokens = tokens->next;
	}
	return (FAILURE);
}

/**
 * @brief Helper function to check whether a redirection token exists in the list.
 *
 * Searches the token list for any redirection operator:
 * - TKN_IN (`<`)
 * - TKN_OUT (`>`)
 * - TKN_APPEND (`>>`)
 * - TKN_HEREDOC (`<<`)
 *
 * @param tokens Pointer to the token list.
 * @return SUCCESS if a redirection is found; otherwise FAILURE.
 */
static int	find_redir(t_tkn *tokens)
{
	while (tokens)
	{
		if (tokens->type == TKN_IN || tokens->type == TKN_OUT
			|| tokens->type == TKN_APPEND || tokens->type == TKN_HEREDOC)
			return (SUCCESS);
		tokens = tokens->next;
	}
	return (FAILURE);
}

/**
 * @brief Performs global syntax validation on the full token list.
 *
 * - If the first token is a pipe, checks for pipe-specific errors.
 * - If any redirection token exists, checks for proper syntax.
 *
 * Sets the shell's exit status to 2 on syntax failure.
 *
 * @param msh Pointer to the shell state.
 * @param tokens Pointer to the full token list.
 * @return FAILURE if a syntax error is detected; otherwise SUCCESS.
 */
int	syntax_check(t_msh *msh, t_tkn *tokens)
{
	if (tokens->type == TKN_PIPE)
		syntax_check_pipes(msh, tokens);
	else if (find_redir(tokens) == SUCCESS)
		if (syntax_check_redirs(msh, tokens) != SUCCESS)
			return (FAILURE);
	return (SUCCESS);
}
