/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   110_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 12:34:09 by luigi             #+#    #+#             */
/*   Updated: 2024/10/18 14:10:52 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	init_msh(t_msh *msh, char **envp)
{
	if (!msh->envp)
		msh->envp = init_env(envp); //get_envp
	// msh->home = //get_variable_home
	// msh->user = 
	// msh->hostname = 
	msh->cmds = NULL;
}

void	init_struct(t_msh *msh)
{
	msh->path = NULL;
	msh->cmd_count = 0;
	msh->exit_status = 0;
}

char	**init_env(char **envp)
{
	char	**env_copy;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	env_copy = malloc(sizeof(char *) * (i + 1));
	if (!env_copy)
		return (NULL);
	i = -1;
	while (envp[++i])
	{
		env_copy[i] = ft_strdup(envp[i]);
		if (!env_copy)
			return (NULL);
	}
	env_copy[i] = NULL;
	return (env_copy);
}
