/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   203_tkns_handlers_3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 11:59:50 by luigi             #+#    #+#             */
/*   Updated: 2025/01/06 20:02:23 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Handles the double output redirection token ('>>').
 *
 * Adds the token `">>"` to the token list. If the token is quoted in the original
 * input (e.g., `"">>"`), it is stored as a quoted string (`">>"`).
 *
 * @param input The raw input string.
 * @param sp Pointer to the token operation structure.
 */
void	handle_double_out_redir(const char *input, t_tkn_op *sp)
{
	if (sp->i > 0 && input[sp->i - 1] == '"')
		sp->argv[(sp->j)++] = ft_strdup("\">>\"");
	else
		sp->argv[(sp->j)++] = ft_strdup(">>");
	(sp->i) += 2;
}

/**
 * @brief Handles the single output redirection token ('>').
 *
 * Takes into account the total number of tokens (`msh->len`) and checks for edge cases
 * involving quotes or null characters. Adds the appropriate token to the token array.
 *
 * - If the token appears quoted (e.g., `">"`), stores it as such.
 * - If malformed or unsupported, the function may return early.
 *
 * @param input The raw input string.
 * @param sp Pointer to the token operation structure.
 * @param msh Pointer to the main shell state structure, used for context like `len`.
 */
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
