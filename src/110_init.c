/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   110_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 12:34:09 by luigi             #+#    #+#             */
/*   Updated: 2024/11/18 10:20:17 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	increment_shlvl(char **envp);

void	init_msh(t_msh *msh, char **envp)
{
	if (!msh->envp)
		msh->envp = init_env(envp);
	msh->cmds = NULL;
	msh->tree_head = NULL;
	init_vector(&msh->tokens, 100);
	increment_shlvl(msh->envp);
}

void	init_struct(t_msh *msh)
{
	msh->path = NULL;
	msh->cmd_count = 0;
	msh->flag_redir = false;
	msh->arg = NULL;
}

void	init_vector(t_vector *vector, size_t size)
{
	vector->count = 0;
	vector->size = size;
	vector->buffer = malloc(sizeof(t_tkn *) * size);
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

static void	increment_shlvl(char **envp)
{
	int			i;
	int			shlvl_value;
	char		*new_shlvl;
	char		*shlvl_str;

	i = 0;
	while (envp[i])
	{
		if (ft_strnstr(envp[i], "SHLVL=", 6))
		{
			shlvl_value = ft_atoi(envp[i] + 6);
			shlvl_value++;
			shlvl_str = ft_itoa(shlvl_value);
			new_shlvl = ft_strjoin("SHLVL=", shlvl_str);
			free(envp[i]);
			envp[i] = new_shlvl;
			free(shlvl_str);
			return ;
		}
		i++;
	}
	envp[i] = ft_strdup("SHLVL=1");
	envp[i + 1] = NULL;
}
