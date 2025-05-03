/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   212_tkns_type_handler_2.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 14:59:11 by mde-agui          #+#    #+#             */
/*   Updated: 2024/12/24 12:18:03 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Classifies a token as TKN_BLANK or TKN_NULL if it is whitespace or empty.
 *
 * - Whitespace characters are: space, tab, vertical tab, carriage return, form feed.
 * - A token with an empty string (`""`) is marked as `TKN_NULL`.
 *
 * @param token Pointer to the token to classify.
 */
void	handle_blank_and_null(t_tkn *token)
{
	if (token->name[0] == ' ' || token->name[0] == '\n'
		|| token->name[0] == '\v' || token->name[0] == '\t'
		|| token->name[0] == '\r' || token->name[0] == '\f')
		token->type = TKN_BLANK;
	else if (token->name[0] == '\0')
		token->type = TKN_NULL;
}

/**
 * @brief Handles classification of pipe tokens (`|`).
 *
 * If the pipe is not quoted in the input, it's marked as `TKN_PIPE`,
 * otherwise it's treated as a literal command (`TKN_CMD`).
 *
 * @param token Pointer to the token to classify.
 * @param line The original input line used to detect quoting context.
 */
void	handle_pipe_token(t_tkn *token, char *line)
{
	if (verify_quotes(token->name[0], line) == FAILURE)
		token->type = TKN_PIPE;
	else
		token->type = TKN_CMD;
}

/**
 * @brief Handles classification of append redirection tokens (`>>`).
 *
 * If not quoted, sets the type to `TKN_APPEND`; otherwise to `TKN_CMD`.
 *
 * @param token Pointer to the token to classify.
 * @param line The original input line used to detect quoting context.
 */
void	handle_append_token(t_tkn *token, char *line)
{
	if (verify_inbetween_quotes(token->name[0], line) == FAILURE)
		token->type = TKN_APPEND;
	else
		token->type = TKN_CMD;
}

/**
 * @brief Handles classification of single output redirection tokens (`>`).
 *
 * If not quoted, sets the type to `TKN_OUT`; otherwise to `TKN_CMD`.
 *
 * @param token Pointer to the token to classify.
 * @param line The original input line used to detect quoting context.
 */
void	handle_out_token(t_tkn *token, char *line)
{
	if (verify_inbetween_quotes(token->name[0], line) == FAILURE)
		token->type = TKN_OUT;
	else
		token->type = TKN_CMD;
}

/**
 * @brief Handles classification of input redirection tokens (`<`).
 *
 * If not quoted, sets the type to `TKN_IN`; otherwise to `TKN_CMD`.
 *
 * @param token Pointer to the token to classify.
 * @param line The original input line used to detect quoting context.
 */
void	handle_in_token(t_tkn *token, char *line)
{
	if (verify_inbetween_quotes(token->name[0], line) == FAILURE)
		token->type = TKN_IN;
	else
		token->type = TKN_CMD;
}
