/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   300_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:28:36 by luigi             #+#    #+#             */
/*   Updated: 2024/10/31 16:19:20 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	to_parse(t_msh *msh, char *line)
{
	t_tkn	*tokens;
	int		i;

	tokens = NULL;
	if (!line)
		return (FAILURE);
	add_history(line);
	msh->cmds = ft_calloc(1, sizeof(t_command));
	if (!msh->cmds)
		return (FAILURE);
	msh->cmds->av = split_input(line);
	if (!msh->cmds->av)
	{
		free_cmds(msh->cmds, 0);
		return (FAILURE);
	}
	msh->cmd_count = ft_matrixlen(msh->cmds->av);
	tokens = tokenizer(msh->cmds->av);
	
	i = -1;
	while (msh->cmds->av[++i])
		printf("Arg number [%d] -> %s\n", i, msh->cmds->av[i]);
	if (msh->cmds->av[0] && ft_strcmp(msh->cmds->av[0], "exit") == 0)
			msh->exit_status = msh_exit(msh->cmds->av, msh);
	if (msh->cmds->av[0] && ft_strcmp(msh->cmds->av[0], "pwd") == 0 && ft_strlen(msh->cmds->av[0]) == 3)
		msh_pwd();
	if (msh->cmds->av[0] && ft_strcmp(msh->cmds->av[0], "echo") == 0)
		msh_echo(msh->cmds->av);
	if (msh->cmds->av[0] && ft_strcmp(msh->cmds->av[0], "cd") == 0)
		msh_cd(msh->cmds->av);
	if (msh->cmds->av[0] && ft_strcmp(msh->cmds->av[0], "export") == 0)
		msh_export(msh->envp);
	if (msh->cmds->av[0] && ft_strcmp(msh->cmds->av[0], "env") == 0)
		msh_env(msh->envp);
	if (msh->cmds->av[0] && ft_strcmp(msh->cmds->av[0], "unset") == 0)
		msh_unset(msh->cmds->av, &(msh->envp));
	if (line)
	{
		free(line);
		line = NULL;
	}
	free_arg(msh->cmds->av);
	free(msh->cmds);
	return (SUCCESS);
}

