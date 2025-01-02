/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   430_exec_more.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:57:51 by luigi             #+#    #+#             */
/*   Updated: 2024/11/14 10:42:42 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	exec_more(t_msh *msh, t_tkn *tokens)
{
	if (tokens->type == TKN_PIPE)
	{
		if (ft_strcmp(tokens->name, "|") != 0)
		{
			syntax_check(msh, tokens);
			return (SUCCESS);
		}
		if (exec_pipe(msh, tokens) != SUCCESS)
			return (FAILURE);
	}
	else
	{
		if (is_bi(tokens))
		{
			exec_bi(tokens, msh);
			return (SUCCESS);
		}
		if (exec_exe(tokens, msh) != SUCCESS)
			return (FAILURE);
	}
	return (SUCCESS);
}
