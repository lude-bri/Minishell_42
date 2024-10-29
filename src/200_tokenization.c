/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   200_tokenization.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:49:13 by luigi             #+#    #+#             */
/*   Updated: 2024/10/29 12:22:46 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*copy_word(const char *input, int start, int end)
{
	int		i;
	int		len;
	char	*word;

	i = 0;
	len = end - start;
	word = (char *)malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	while (start < end)
		word[i++] = input[start++];
	word[i] = '\0';
	//word[i] = input[start++];
	return (word);
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\n' || c == '\t');
}

int	count_words(const char *input)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	while (input[i])
	{
		while (input[i] && is_whitespace(input[i]))
			i++;
		if (input[i] == '\'')
		{
			i++;
			while (input[i] && input[i] != '\'')
				i++;
			i++;
			counter++;
		}
		else if (input[i] == '"')
		{
			i++;
			while (input[i] && input[i] == '"')
				i++;
			i++;
			counter++;
		}
		else if (input[i])
		{
			counter++;
			while (input[i] && input[i] != '\'' && !is_whitespace(input[i]))
				i++;
		}
	}
	return (counter);
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

char	**split_input(const char *input)
{
	t_tkn	split;
	int		i;
	int		j;

	split.number_words = count_words(input);
	split.argv = (char **)malloc(sizeof(char *) * (split.number_words + 1));
	i = 0;
	j = 0;
	split.start = 0;
	if (!split.argv)
		return (NULL);
	while (input[i])
	{
		while (input[i] && is_whitespace(input[i]))
			i++;
		//handle_single_quotes
		if (input[i] == '\'')
			split.argv[j++] = handle_single_quotes(input, &i);
		//handle_double_quotes
		//handle the rest
		else if (input[i] && !is_whitespace(input[i]))
		{
			split.start = i;
			while (input[i] && input[i] != '\'' && !is_whitespace(input[i]))
				i++;
			split.argv[j++] = copy_word(input, split.start, i);
		}
	}
	split.argv[j] = NULL;
	return (split.argv);
}
