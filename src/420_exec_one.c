/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   420_exec_one.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:40:02 by luigi             #+#    #+#             */
/*   Updated: 2024/11/06 11:41:54 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	is_bi(t_tkn *tokens)
{
	if (tokens->cmd_type == CMD_CD)
		return (SUCCESS);
	if (tokens->cmd_type == CMD_PWD)
		return (SUCCESS);
	if (tokens->cmd_type == CMD_ENV)
		return (SUCCESS);
	if (tokens->cmd_type == CMD_ECHO)
		return (SUCCESS);
	if (tokens->cmd_type == CMD_EXIT)
		return (SUCCESS);
	if (tokens->cmd_type == CMD_UNSET)
		return (SUCCESS);
	if (tokens->cmd_type == CMD_EXPORT)
		return (SUCCESS);
	else
		return (FAILURE);
}

// int	exec_one(t_msh *msh, t_tkn *tokens)
// {
// 	if (tokens->type == TKN_CMD)
// 		if (exec_bi(tokens, msh) == FAILURE)
// 			if (exec_exe(tokens, msh) != SUCCESS)
// 				return (FAILURE);
// 	return (SUCCESS);
// }

int	exec_one(t_msh *msh, t_tkn *tokens)
{
	if (tokens->type == TKN_CMD)
	{
		if (is_bi(tokens) == SUCCESS)
			exec_bi(tokens, msh);
		else
			exec_exe(tokens, msh);
	}
	return (SUCCESS);
}
