/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   700_redirs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 09:09:40 by luigi             #+#    #+#             */
/*   Updated: 2024/12/23 02:50:09 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	flags_check(t_tkn *tokens)
{
	if (ft_strchr(tokens->next->next->name, '-') != 0)
		printf("and it is exec\n");
	else
		printf("and it is not exec\n");
}

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

static int	redir_found(t_tkn *tokens)
{
	return ((tokens->type == TKN_IN || tokens->type == TKN_OUT
			|| tokens->type == TKN_APPEND));
}

int	exec_redirs(t_tkn *tokens, t_msh *msh)
{
	t_tkn	*tkn;

	tkn = tokens;
	while (tkn)
	{
		if (tkn->type == TKN_HEREDOC && !redir_found(tkn))
		{
			heredoc(tokens, msh, tkn->next->name, 0);
			return (SUCCESS);
		}
		if (tkn->type == TKN_IN || tkn->type == TKN_OUT
			|| tkn->type == TKN_APPEND)
		{
			redirs_2(tokens, msh);
		}
		tkn = tkn->next;
	}
	return (SUCCESS);
}
