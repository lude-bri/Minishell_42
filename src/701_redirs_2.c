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

void	handle_command_arguments(t_tkn *tokens, t_msh *msh)
{
	int	i;

	i = 0;
	msh->arg = ft_calloc(tokens->len + 1, sizeof(char *));
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

void	redirs_2(t_tkn *tokens, t_msh *msh)
{
	handle_redirs(tokens, msh);
	handle_command_arguments(tokens, msh);
}
