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

static void	restruct_cl(t_tkn *tokens, t_msh *msh)
{
	//function to restructure the command line
	//organize command "redir" file
	(void)tokens;
	(void)msh;
	printf("restruct redir\n");
}

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

int	exec_one(t_msh *msh, t_tkn *tokens)
{
	char	*path;

	if (tokens->type == TKN_CMD)
	{
		if (is_bi(tokens) == SUCCESS)
			exec_bi(tokens, msh);
		else
			exec_exe(tokens, msh);
	}
	else if ((tokens->type == TKN_IN || tokens->type == TKN_OUT))
	{
		if (tokens->next != NULL)
		{
			printf("redir in the beginning found\n");
			path = find_path(tokens->next->name, msh->envp);
			if (!path)
			{
				restruct_cl(tokens, msh);
				return (SUCCESS);
			}
			else
			{
				redirs(tokens, msh);
				exec_one(msh, tokens->next);
			}
		}
	}
	return (SUCCESS);
}

// int	exec_one(t_msh *msh, t_tkn *tokens)
// {
// 	if (tokens->type == TKN_CMD)
// 		if (exec_bi(tokens, msh) == FAILURE)
// 			if (exec_exe(tokens, msh) != SUCCESS)
// 				return (FAILURE);
// 	return (SUCCESS);
// }
