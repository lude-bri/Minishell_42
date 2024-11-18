/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   700_redirs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 09:09:40 by luigi             #+#    #+#             */
/*   Updated: 2024/11/18 08:16:07 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void redirs(t_tkn *tokens, t_msh *msh)
{
	t_tkn	*tkn_exec;

	tkn_exec = tokens;
    if (tokens->type == TKN_IN)
		redir_in(tkn_exec, msh);
    else if (tokens->type == TKN_OUT)
		redir_out(tkn_exec, msh);
    else if (tokens->type == TKN_APPEND)
		redir_append(tkn_exec, msh);
}

int	exec_redirs(t_tkn *tokens, t_msh *msh)
{
	while (tokens)
	{
		if (tokens->type == TKN_HEREDOC)
		{
			heredoc(tokens, msh);
			return (SUCCESS);
		}
		if (tokens->type == TKN_IN || tokens->type == TKN_OUT)
		{
			redirs(tokens, msh);
			return (SUCCESS);
		}
		tokens = tokens->next;
	}
	return (SUCCESS);
}
