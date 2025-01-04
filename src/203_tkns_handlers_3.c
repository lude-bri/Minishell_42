/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   203_tkns_handlers_3.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/24 11:59:50 by luigi             #+#    #+#             */
/*   Updated: 2025/01/04 20:47:51 by mde-agui         ###   ########.fr       */
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

static void    pr_quote(const char *input, t_quote *qd, t_msh *msh, char quote)
{
    char    *expanded;

    if ((*qd->buf_i > 0 || quote == '"') && (*qd->i != 0))
        (*qd->i)++;
    
    while (input[*qd->i] && input[*qd->i] != quote)
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
        {
            qd->buffer[(*qd->buf_i)++] = input[(*qd->i)++];
        }
    }
    
    if (input[*qd->i] == quote)
        (*qd->i)++;
}


// Modified handle_words function
void    handle_words(const char *input, t_tkn_op *sp, t_msh *msh)
{
    char    buffer[BUF_SIZE];
    int     buf_i;
    t_quote qd;
    char    *expanded;
    bool    is_first_word;

    ft_memset(buffer, 0, BUF_SIZE);
    buf_i = 0;
    is_first_word = (sp->j == 0);

    // Special handling for quoted first word
    if (is_first_word && (input[sp->i] == '"' || input[sp->i] == '\''))
    {
        char quote_char = input[sp->i];
        sp->i++; // Skip opening quote
        while (input[sp->i] && input[sp->i] != quote_char)
            buffer[buf_i++] = input[sp->i++];
        if (input[sp->i] == quote_char)
            sp->i++; // Skip closing quote
        buffer[buf_i] = '\0';
        sp->argv[sp->j++] = ft_strdup(buffer);
        return;
    }

    while (input[sp->i] && !is_whitespace(input[sp->i])
        && input[sp->i] != '|' && input[sp->i] != '>' && input[sp->i] != '<')
    {
        if (input[sp->i] == '"' || input[sp->i] == '\'')
        {
            qd = (t_quote){buffer, &buf_i, &sp->i};
            pr_quote(input, &qd, msh, input[sp->i]);
        }
        else if (input[sp->i] == '$')
        {
            qd = (t_quote){buffer, &buf_i, &sp->i};
            expanded = expand_var(input, qd.i, msh);
            if (expanded)
            {
                if (!(is_first_word && buf_i == 0 && expanded[0] == '\0'))
                {
                    ft_strncat(qd.buffer, expanded, BUF_SIZE - *qd.buf_i - 1);
                    *qd.buf_i = ft_strlen(qd.buffer);
                }
                free(expanded);
            }
        }
        else
            buffer[buf_i++] = input[sp->i++];
    }

    if (buf_i > 0 || !is_first_word)
    {
        buffer[buf_i] = '\0';
        sp->argv[sp->j++] = ft_strdup(buffer);
    }
}
