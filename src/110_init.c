/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   110_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 12:34:09 by luigi             #+#    #+#             */
/*   Updated: 2025/01/02 16:08:05 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	init_heredoc(t_heredoc *heredoc)
{
	heredoc->i = 0;
	heredoc->count_hd = 0;
	heredoc->fd_heredoc_path = NULL;
	heredoc->eof = NULL;
	heredoc->next = NULL;
}

void	init_msh(t_msh *msh, char **envp)
{
	if (!msh->envp)
	{
		msh->envp = init_env(envp);
		msh->ex_envp = init_env(envp);
	}
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
	msh->flag_exit = false;
	msh->arg = NULL;
	msh->heredoc = ft_calloc(1, sizeof(t_heredoc));
	init_heredoc(msh->heredoc);
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
