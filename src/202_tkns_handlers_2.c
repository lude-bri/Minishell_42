/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   202_tkns_handlers_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 23:13:00 by mde-agui          #+#    #+#             */
/*   Updated: 2024/12/22 01:34:36 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*handle_double_quotes(const char *input, int *i, t_msh *msh)
{
	msh->word_size = ft_strlen(input) * 1024;
	msh->word = (char *)malloc(msh->word_size);
	msh->word[0] = '\0';
	while (input[*i] && input[*i] != '"')
	{
		if (input[*i] == '$' && input[*i + 1] != ' ' && input[*i + 2] != '\0')
		{
			if (!expand_helper(input, i, msh))
				break ;
		}
		else
			append_char_to_word(input, i, msh);
	}
	if (input[*i] == '"')
		(*i)++;
	return (msh->word);
}

char	*handle_single_quotes(const char *input, int *i)
{
	int		start;
	char	*word;

	start = ++(*i);
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	word = copy_word(input, start, *i);
	if (input[*i] == '\'')
		(*i)++;
	return (word);
}

void	handle_words(const char *input, t_tkn_op *sp)
{
	sp->start = sp->i;
	while (input[sp->i] && input[sp->i] != '\'' && input[sp->i] != '"'
		&& input[sp->i] != '|' && input[sp->i] != '>' && input[sp->i] != '<'
		&& !is_whitespace(input[sp->i]))
		(sp->i)++;
	sp->argv[(sp->j)++] = copy_word(input, sp->start, sp->i);
}

void	handle_double_in_redir(const char *input, t_tkn_op *sp)
{
	if (sp->i > 0 && input[sp->i - 1] == '"')
		sp->argv[(sp->j)++] = ft_strdup("\"<<\"");
	else
		sp->argv[(sp->j)++] = ft_strdup("<<");
	(sp->i) += 2;
}

void	handle_single_in_redir(const char *input, t_tkn_op *sp, t_msh *msh)
{
	if (msh->len == 1)
	{
		(sp->i)++;
		sp->argv[(sp->j)++] = ft_strdup("<");
		return ;
	}
	else if (msh->len == 2)
	{
		if (sp->i > 0 && input[sp->i - 1] == '\0')
		{
			if (input[sp->i + 1] == '"')
				sp->argv[(sp->j)++] = ft_strdup("\"<\"");
			else
				return ;
		}
	}
	else
		sp->argv[(sp->j)++] = ft_strdup("<");
	(sp->i)++;
}
