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

void	handle_blank_and_null(t_tkn *token)
{
	if (token->name[0] == ' ' || token->name[0] == '\n'
		|| token->name[0] == '\v' || token->name[0] == '\t'
		|| token->name[0] == '\r' || token->name[0] == '\f')
		token->type = TKN_BLANK;
	else if (token->name[0] == '\0')
		token->type = TKN_NULL;
}

void	handle_pipe_token(t_tkn *token, char *line)
{
	if (verify_quotes(token->name[0], line) == FAILURE)
		token->type = TKN_PIPE;
	else
		token->type = TKN_CMD;
}

void	handle_append_token(t_tkn *token, char *line)
{
	if (verify_inbetween_quotes(token->name[0], line) == FAILURE)
		token->type = TKN_APPEND;
	else
		token->type = TKN_CMD;
}

void	handle_out_token(t_tkn *token, char *line)
{
	if (verify_inbetween_quotes(token->name[0], line) == FAILURE)
		token->type = TKN_OUT;
	else
		token->type = TKN_CMD;
}

void	handle_in_token(t_tkn *token, char *line)
{
	if (verify_inbetween_quotes(token->name[0], line) == FAILURE)
		token->type = TKN_IN;
	else
		token->type = TKN_CMD;
}
