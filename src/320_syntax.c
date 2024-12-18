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

static int	find_redir(t_tkn *tokens);
static int	find_cmd(t_tkn *tokens);

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

static int	find_cmd(t_tkn *tokens)
{
	t_tkn	*current;

	current = NULL;
	while (tokens)
	{
		if (!current && tokens->type == TKN_CMD)
			return (SUCCESS);
		else if (current && (current->type == TKN_IN || current->type == TKN_OUT
			|| current->type == TKN_APPEND || current->type == TKN_HEREDOC) 
				&& (tokens->type == TKN_CMD))
		{
			if (current->type == TKN_APPEND || current->type == TKN_HEREDOC)
				current = current->next;
			else if (current->type == TKN_CMD)
				return (SUCCESS);
		}
		else if (current && current->type == TKN_CMD)
			return (SUCCESS);
		current = tokens;
		tokens = tokens->next;
	}
	return (FAILURE);
	// while (tokens)
	// {
	// 	if (tokens->type == TKN_CMD)
	// 		return (SUCCESS);
	// 	tokens = tokens->next;
	// }
	// return (FAILURE);
}


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

int	syntax_check(t_msh *msh, t_tkn *tokens)
{
	if (tokens->type == TKN_PIPE)
		syntax_check_pipes(msh, tokens);
	// else if ((tokens->type == TKN_IN || tokens->type == TKN_OUT
	// 	|| tokens->type == TKN_HEREDOC)
	// 	&& tokens->next == NULL)
	else if (find_redir(tokens) == SUCCESS)
		if (syntax_check_redirs(msh, tokens) != SUCCESS)
			return (FAILURE);
	return (SUCCESS);
}
