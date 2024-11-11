/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   320_syntax.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 11:27:09 by luigi             #+#    #+#             */
/*   Updated: 2024/11/11 11:44:17 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	syntax_check(t_msh *msh, t_tkn *tokens)
{
	if (tokens->type == TKN_PIPE)
	{
		printf("msh: syntax error near unexpected token `%s'\n", tokens->name);
		msh->exit_status = 2;
		return (FAILURE);
	}
	else if (tokens->type == TKN_IN || tokens->type == TKN_OUT)
	{
		printf("msh: syntax error near unexpected token `newline'\n");
		msh->exit_status = 2;
		return (FAILURE);
	}
	return (SUCCESS);
}
