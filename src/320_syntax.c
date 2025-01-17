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
	else if (find_redir(tokens) == SUCCESS)
		if (syntax_check_redirs(msh, tokens) != SUCCESS)
			return (FAILURE);
	return (SUCCESS);
}
