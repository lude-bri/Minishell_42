/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   203_tkns_misc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 23:16:03 by mde-agui          #+#    #+#             */
/*   Updated: 2024/12/22 01:33:29 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	quote_helper(const char *input, int i)
{
	char	quote;

	quote = input[i++];
	while (input[i] && input[i] != quote)
		i++;
	if (input[i])
		i++;
	return (i);
}

bool	special_char_helper(const char *input, int *i)
{
	if (input[*i] == '|' || input[*i] == '>'
		|| input[*i] == '<' || input[*i] == '$')
	{
		(*i)++;
		return (true);
	}
	return (false);
}

bool	regular_char_helper(const char *input, int *i)
{
	bool	incr;

	incr = false;
	while (input[*i] && input[*i] != '\'' && input[*i] != '"'
		&& input[*i] != '|' && input[*i] != '<' && input[*i] != '>'
		&& !is_whitespace(input[*i]))
	{
		(*i)++;
		incr = true;
	}
	return (incr);
}

void	append_char_to_word(const char *input, int *i, t_msh *msh)
{
	size_t	len;

	len = ft_strlen(msh->word);
	if (len + 1 < msh->word_size - 1)
	{
		msh->word[len] = input[*i];
		msh->word[len + 1] = '\0';
	}
	(*i)++;
}

int	take_len(const char *input)
{
	int	i;

	i = 0;
	while (*input && is_whitespace(input[i]))
		input++;
	while (input[i])
		i++;
	return (i);
}
