/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   320_syntax.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 11:27:09 by luigi             #+#    #+#             */
/*   Updated: 2024/11/14 11:11:42 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

int	syntax_check_redirs(t_msh *msh, t_tkn *tokens)
{
	(void)tokens;
	printf("msh: syntax error near unexpected token `newline'\n");
	msh->exit_status = 2;
	return (FAILURE);
}

int	syntax_check(t_msh *msh, t_tkn *tokens)
{
	if (tokens->type == TKN_PIPE)
		syntax_check_pipes(msh, tokens);
	else if (tokens->type == TKN_IN || tokens->type == TKN_OUT)
		syntax_check_redirs(msh, tokens);
	return (SUCCESS);
}
