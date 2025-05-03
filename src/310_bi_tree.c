/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   310_bi_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 10:21:43 by luigi             #+#    #+#             */
/*   Updated: 2024/11/07 11:02:45 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Recursively builds a binary tree of tokens separated by pipes.
 *
 * This function traverses a linked list of tokens until it finds a `TKN_PIPE` token.
 * When a pipe is found, it becomes the root of a subtree:
 * - `left` points to the list before the pipe (command before the pipe),
 * - `right` is built recursively from the list after the pipe.
 *
 * The final tree structure reflects the execution order of piped commands.
 *
 * @param tokens Pointer to the head of the token list.
 * @return The root of the binary tree representing piped commands.
 */
t_tkn	*create_bin_tree(t_tkn *tokens)
{
	t_tkn	*current;

	if (!tokens)
		return (NULL);
	current = tokens;
	while (current && current->type != TKN_PIPE)
		current = current->next;
	if (current && current->type == TKN_PIPE)
	{
		current->left = tokens;
		current->right = create_bin_tree(current->next);
		tokens = current;
	}
	return (tokens);
}
