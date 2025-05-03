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

/**
 * @brief Initializes the heredoc structure.
 *
 * Sets default values for all fields of a t_heredoc instance.
 *
 * @param heredoc Pointer to the t_heredoc structure to initialize.
 */
static void	init_heredoc(t_heredoc *heredoc)
{
	heredoc->i = 0;
	heredoc->count_hd = 0;
	heredoc->fd_heredoc_path = NULL;
	heredoc->eof = NULL;
	heredoc->next = NULL;
}

/**
 * @brief Initializes the main minishell structure with environment variables and token vector.
 *
 * Allocates memory and sets initial values for environment arrays, command list,
 * execution tree, and token buffer. Also increments the shell level (SHLVL).
 *
 * @param msh Pointer to the t_msh structure to initialize.
 * @param envp Array of strings containing the system environment variables.
 */
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

/**
 * @brief Resets transient fields in the t_msh structure for a new shell iteration.
 *
 * Clears path-related fields, flags, argument lists, and allocates a new heredoc structure.
 *
 * @param msh Pointer to the t_msh structure to reset.
 */
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

/**
 * @brief Initializes a dynamic vector structure to store token pointers.
 *
 * Allocates a buffer with the given initial capacity and resets counters.
 *
 * @param vector Pointer to the t_vector structure.
 * @param size Initial capacity of the vector.
 */
void	init_vector(t_vector *vector, size_t size)
{
	vector->count = 0;
	vector->size = size;
	vector->buffer = malloc(sizeof(t_tkn *) * size);
}

/**
 * @brief Creates a duplicate of the system environment variables.
 *
 * Allocates and copies each string from the original environment array into a new array.
 *
 * @param envp Original environment variable array.
 * @return A newly allocated null-terminated array of duplicated strings, or NULL on failure.
 */
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
