/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   742_heredoc_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 15:58:39 by luigi             #+#    #+#             */
/*   Updated: 2025/01/02 16:00:08 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	find_heredoc(t_tkn *tokens)
{
	t_tkn	*tkn;

	tkn = tokens;
	while (tkn)
	{
		if (tkn->type == TKN_HEREDOC)
			return (SUCCESS);
		tkn = tkn->next;
	}
	return (FAILURE);
}
