/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   400_execute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:32:38 by luigi             #+#    #+#             */
/*   Updated: 2024/10/31 15:56:02 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	to_execute(char *command)
{
	(void)command;
	return (0);
}

	//
	//
	// i = -1;
	// while (msh->cmds->av[++i])
	// 	printf("Arg number [%d] -> %s\n", i, msh->cmds->av[i]);
	// if (msh->cmds->av[0] && ft_strcmp(msh->cmds->av[0], "exit") == 0)
	// 		msh_exit(msh->cmds->av, msh);
	// if (msh->cmds->av[0] && ft_strcmp(msh->cmds->av[0], "pwd") == 0 && ft_strlen(msh->cmds->av[0]) == 3)
	// 	msh_pwd();
	// if (msh->cmds->av[0] && ft_strcmp(msh->cmds->av[0], "echo") == 0)
	// 	msh_echo(msh->cmds->av);
	// if (msh->cmds->av[0] && ft_strcmp(msh->cmds->av[0], "cd") == 0)
	// 	msh_cd(msh->cmds->av);
	// if (msh->cmds->av[0] && ft_strcmp(msh->cmds->av[0], "export") == 0)
	// 	msh_export(msh->envp);
	// if (msh->cmds->av[0] && ft_strcmp(msh->cmds->av[0], "env") == 0)
	// 	msh_env(msh->envp);
	// if (msh->cmds->av[0] && ft_strcmp(msh->cmds->av[0], "unset") == 0)
	// 	msh_unset(msh->cmds->av, &(msh->envp));
	// if (line)
	// {
	// 	free(line);
	// 	line = NULL;
	// }
	//
