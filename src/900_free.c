/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   900_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 12:53:23 by luigi             #+#    #+#             */
/*   Updated: 2025/01/06 20:09:22 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Frees all nodes in the heredoc linked list.
 *
 * Also frees any allocated string used as the heredoc temporary file path.
 *
 * @param heredoc Pointer to the head of the t_heredoc list.
 */
void	free_heredoc(t_heredoc *heredoc)
{
	t_heredoc	*tmp;

	while (heredoc)
	{
		tmp = heredoc->next;
		if (heredoc->fd_heredoc_path)
		{
			free(heredoc->fd_heredoc_path);
			heredoc->fd_heredoc_path = NULL;
		}
		free(heredoc);
		heredoc = NULL;
		heredoc = tmp;
	}
}

/**
 * @brief Frees all tokens stored in the dynamic token vector.
 *
 * Each token's `name` and the token itself are freed, followed by the vector buffer.
 * Resets the vector's size and count to 0.
 *
 * @param vector Pointer to the t_vector structure holding tokens.
 */
void	free_vector(t_vector *vector)
{
	int		i;
	t_tkn	*token;

	i = 0;
	while (i < vector->count)
	{
		token = vector->buffer[i];
		if (token != NULL)
		{
			free(token->name);
			free(token);
			token = NULL;
		}
		i++;
	}
	if (vector->buffer != NULL)
		free(vector->buffer);
	vector->buffer = NULL;
	vector->count = 0;
	vector->size = 0;
}

/**
 * @brief Frees a NULL-terminated array of strings.
 *
 * @param str The array of strings to free.
 * @param error Return value for consistency in error handling.
 * @return int Returns the `error` parameter.
 */
int	free_array(char **str, int error)
{
	int		i;

	if (!str)
		return (error);
	i = -1;
	while (str[++i])
		free(str[i]);
	free(str);
	return (error);
}

/**
 * @brief Frees a NULL-terminated array of arguments.
 *
 * Each individual string is freed, then the array itself.
 *
 * @param argv Array of strings to be freed.
 */
void	free_arg(char **argv)
{
	int	i;

	i = 0;
	if (!argv)
		return ;
	while (argv[i])
	{
		free(argv[i]);
		argv[i] = NULL;
		i++;
	}
	free(argv);
	argv = NULL;
}

/**
 * @brief Frees all resources associated with the msh runtime context.
 *
 * Frees command arguments, input line, tokens vector, heredoc list, and command struct.
 *
 * @param command Pointer to the current command structure.
 * @param msh Pointer to the main shell state structure.
 * @param token Pointer to token list (used to determine if vector should be freed).
 */
void	free_msh(t_command *command, t_msh *msh, t_tkn *token)
{
	if (msh->cmds->av)
		free_arg(msh->cmds->av);
	if (msh->line)
		free(msh->line);
	if (token)
		free_vector(&msh->tokens);
	if (command)
		free(command);
	if (msh->heredoc)
	{
		free_heredoc(msh->heredoc);
		msh->heredoc = NULL;
	}
}
