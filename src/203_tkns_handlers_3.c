/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   203_tkns_handlers_3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 11:59:50 by luigi             #+#    #+#             */
/*   Updated: 2024/12/24 12:20:17 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_double_out_redir(const char *input, t_tkn_op *sp)
{
	if (sp->i > 0 && input[sp->i - 1] == '"')
		sp->argv[(sp->j)++] = ft_strdup("\">>\"");
	else
		sp->argv[(sp->j)++] = ft_strdup(">>");
	(sp->i) += 2;
}

void	handle_single_out_redir(const char *input, t_tkn_op *sp, t_msh *msh)
{
	if (input[sp->i] == '>')
	{
		if (msh->len == 1)
		{
			(sp->i)++;
			sp->argv[(sp->j)++] = ft_strdup(">");
			return ;
		}
		else if (msh->len == 2)
		{
			if (sp->i > 0 && input[sp->i - 1] == '\0')
			{
				if (input[sp->i + 1] == '"')
					sp->argv[(sp->j)++] = ft_strdup("\">\"");
				else
					return ;
			}
		}
		else
			sp->argv[(sp->j)++] = ft_strdup(">");
		(sp->i)++;
	}
}
