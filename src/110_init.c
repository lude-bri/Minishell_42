/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   110_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 12:34:09 by luigi             #+#    #+#             */
/*   Updated: 2024/10/18 13:04:18 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**init_envp(char **envp)
{
	char	**msh_envp;
	int		i;

	i = 0;
	while (envp[0])
		i++;
	msh_envp = ft_calloc(sizeof(char *), i + 1);
	if (!msh_envp)
		return (NULL);
	i = -1;
	while (envp[++i])
		msh_envp[i] = ft_strdup(envp[i]);
	msh_envp[i] = NULL;
	return (msh_envp);
}

void	init_struct(t_msh *msh)
{
	msh->path = NULL;
	msh->cmd_count = 0;
	msh->exit_status = 0;
}
