/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenization.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:49:13 by luigi             #+#    #+#             */
/*   Updated: 2024/10/23 12:49:15 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// static char	**free_arrays(const char **s, int a);

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
	word[i] = input[start++];
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
		if (input[i])
			counter++;
		while (input[i] && !is_whitespace(input[i]))
			i++;
	}
	return (counter);
}

// static char	**free_arrays(const char **s, int a)
// {
// 	while (a > 0)
// 	{
// 		a--;
// 		free((void *)s[a]);
// 	}
// 	free(s);
// 	return (NULL);
// }

char	**split_input(const char *input)
{
	int		number_words;
	char	**argv;
	int		i;
	int		j;
	int		start;

	number_words = count_words(input);
	argv = (char **)malloc(sizeof(char *) * (number_words + 1));
	i = 0;
	j = 0;
	start = 0;
	if (!argv)
		return (NULL);
	while (input[i])
	{
		while (input[i] && is_whitespace(input[i]))
			i++;
		start = i;
		while (input[i] && !is_whitespace(input[i]))
			i++;
		if (start < i)
			argv[j++] = copy_word(input, start, i);
	}
	argv[j] = NULL;
	return (argv);
}
