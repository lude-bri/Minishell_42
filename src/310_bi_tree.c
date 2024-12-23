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
