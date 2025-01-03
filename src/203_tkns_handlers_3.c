/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   203_tkns_handlers_3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 11:59:50 by luigi             #+#    #+#             */
/*   Updated: 2025/01/03 14:15:32 by mde-agui         ###   ########.fr       */
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

static void	pr_quote(const char *input, t_quote *qd, t_msh *msh, char quote)
{
	char	*expanded;

	if ((*qd->buf_i > 0 || quote == '"') && (*qd->i != 0))
		(*qd->i)++;
	while (input[*qd->i] && input[*qd->i] != quote
		&& !is_whitespace(input[*qd->i]))
	{
		if (quote == '"' && input[*qd->i] == '$')
		{
			expanded = expand_var(input, qd->i, msh);
			if (expanded)
			{
				ft_strncat(qd->buffer, expanded, BUF_SIZE - *qd->buf_i - 1);
				*qd->buf_i = ft_strlen(qd->buffer);
				free(expanded);
			}
		}
		else
			qd->buffer[(*qd->buf_i)++] = input[(*qd->i)++];
	}
	if (*qd->buf_i > 0 || quote == '"')
	{
		if (input[*qd->i] == quote)
			(*qd->i)++;
	}
}

void	handle_words(const char *input, t_tkn_op *sp, t_msh *msh)
{
	char	buffer[BUF_SIZE];
	int		buf_i;
	t_quote	qd;

	ft_memset(buffer, 0, BUF_SIZE);
	buf_i = 0;
	while (input[sp->i] && !is_whitespace(input[sp->i])
		&& input[sp->i] != '|' && input[sp->i] != '>' && input[sp->i] != '<')
	{
		if (input[sp->i] == '"' || input[sp->i] == '\'')
		{
			qd = (t_quote){buffer, &buf_i, &sp->i};
			pr_quote(input, &qd, msh, input[sp->i]);
		}
		else
			buffer[buf_i++] = input[sp->i++];
	}
	buffer[buf_i] = '\0';
	sp->argv[sp->j++] = ft_strdup(buffer);
}
