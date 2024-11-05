/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   400_execute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:32:38 by luigi             #+#    #+#             */
/*   Updated: 2024/10/31 16:09:07 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	exec_bi(t_tkn *tokens, char **command, t_msh *msh);

int	to_execute(char **command, t_msh *msh, t_tkn *tokens)
{
	if (tokens->type == TKN_CMD)
		exec_bi(tokens, command, msh);
	return (SUCCESS);
}

static int	exec_bi(t_tkn *tokens, char **command, t_msh *msh)
{
	if (tokens->cmd_type == CMD_CD)
		msh_cd(command);
	if (tokens->cmd_type == CMD_ENV)
		msh_env(msh->envp);
	if (tokens->cmd_type == CMD_PWD)
		msh_pwd();
	if (tokens->cmd_type == CMD_ECHO)
		msh_echo(command);
	if (tokens->cmd_type == CMD_EXIT)
		msh_exit(command, msh);
	if (tokens->cmd_type == CMD_UNSET)
		msh_unset(command, &(msh->envp));
	if (tokens->cmd_type == CMD_EXPORT)
		msh_export(msh->envp);
	return (SUCCESS);
}






/////////////////////////////////
////         OLD EXEC        ////
/////////////////////////////////

	// i = -1;
	// while (command[++i])
	// 	printf("Arg number [%d] -> %s\n", i, command[i]);
	// if (command[0] && ft_strcmp(command[0], "exit") == 0)
	// 		msh_exit(command, msh);
	// if (command[0] && ft_strcmp(command[0], "pwd") == 0)
	// 	msh_pwd();
	// if (command[0] && ft_strcmp(command[0], "echo") == 0)
	// 	msh_echo(command);
	// if (command[0] && ft_strcmp(command[0], "cd") == 0)
	// 	msh_cd(command);
	// if (command[0] && ft_strcmp(command[0], "export") == 0)
	// 	msh_export(msh->envp);
	// if (command[0] && ft_strcmp(command[0], "env") == 0)
	// 	msh_env(msh->envp);
	// if (command[0] && ft_strcmp(command[0], "unset") == 0)
	// 	msh_unset(command, &(msh->envp));
	// return (SUCCESS);
