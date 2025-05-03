/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   401_build_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 12:18:32 by luigi             #+#    #+#             */
/*   Updated: 2024/12/23 00:11:09 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Counts how many tokens belong to the current command segment.
 *
 * Iterates through the token list and counts tokens until a pipe (`TKN_PIPE`)
 * or the end is reached. Used to determine the size of the argument array.
 *
 * @param tokens Pointer to the beginning of the token list.
 * @return Number of command tokens before a pipe.
 */
static int	count_tokens(t_tkn *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == TKN_PIPE)
			break ;
		else
		{
			count++;
			tokens = tokens->next;
		}
	}
	return (count);
}

/**
 * @brief Frees a partially allocated argument array.
 *
 * Used to clean up in case of memory allocation failure during
 * argument copying.
 *
 * @param args The array of strings to free.
 * @param count Number of elements currently allocated.
 */
void	free_args(char **args, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(args[i]);
		i++;
	}
	free(args);
}

/**
 * @brief Allocates and fills an argument array from the token list.
 *
 * Copies the `name` field from each token into a new string.
 * Stops copying when encountering a pipe or redirection token.
 *
 * @param tokens Pointer to the start of the token list.
 * @param num_tokens Number of arguments to allocate (from count_tokens).
 * @return A NULL-terminated array of strings on success, or NULL on failure.
 */
char	**allocate_and_copy_args(t_tkn *tokens, int num_tokens)
{
	char	**args;
	int		i;

	args = (char **)malloc(sizeof(char *) * (num_tokens + 1));
	i = 0;
	if (!args)
		return (NULL);
	while (tokens)
	{
		if (tokens->type == TKN_PIPE || tokens->type == TKN_IN
			|| tokens->type == TKN_OUT || tokens->type == TKN_APPEND
			|| tokens->type == TKN_HEREDOC)
			break ;
		args[i] = ft_strdup(tokens->name);
		if (!args[i])
			return (free_args(args, i), NULL);
		i++;
		tokens = tokens->next;
	}
	args[i] = NULL;
	return (args);
}

/**
 * @brief Builds the argument array for the current command.
 *
 * Combines `count_tokens()` and `allocate_and_copy_args()` to create
 * the argument list used by `execve()` or built-in command handlers.
 *
 * @param tokens Pointer to the start of the token list.
 * @return A NULL-terminated array of arguments, or NULL on allocation failure.
 */
char	**build_args(t_tkn *tokens)
{
	int	num_tokens;

	num_tokens = count_tokens(tokens);
	return (allocate_and_copy_args(tokens, num_tokens));
}
