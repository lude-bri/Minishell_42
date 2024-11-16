/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   700_redirs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 09:09:40 by luigi             #+#    #+#             */
/*   Updated: 2024/11/16 10:20:58 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	exec_redirs(t_tkn *tokens, t_msh *msh)
{
	(void)msh;
	//printf("redirec activated\n");
	while (tokens)
	{
		if (tokens->type == TKN_HEREDOC)
		{
			printf("heredoc found\n");
			return (SUCCESS);
		}
		if (tokens->type == TKN_IN || tokens->type == TKN_OUT)
		{
			printf("redirect found\n");
			return (SUCCESS);
		}
		tokens = tokens->next;
	}
	return (FAILURE);
}
