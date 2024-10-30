/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   200_tokenization.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:49:13 by luigi             #+#    #+#             */
/*   Updated: 2024/10/29 17:32:58 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// static char	**free_arrays(const char **s, int a);

char	*expand_var(const char *input, int *i)
{
	int		len;
	int		start;
	char	*var_name;
	char	*value;
	char	*result;

	len = 0;
	start = *i + 1;
	while (input[start + len] && (ft_isalnum(input[start + len]) || input[start + len] == '_'))
		len++;
	var_name = (char *)malloc(len + 1);
	if (!var_name)
		return (NULL);
	ft_strlcpy(var_name, &input[start], len + 1);
	var_name[len] = '\0';
	value = getenv(var_name);
	free(var_name);
	if (value)
		result = ft_strdup(value);
	else
		result = ft_strdup("");
	*i += len + 1;
	return (result);
}

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
			if (!input[i])
				break ;
			i++;
			counter++;
		}
		else if (input[i] == '"')
		{
			i++;
			while (input[i] && input[i] == '"')
				i++;
			if (!input[i])
				break ;
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

char	*handle_double_quotes(const char *input, int *i)
{
	size_t	word_size;
	size_t	len;
	char	*word;
	char	*expanded;
	
	word_size = ft_strlen(input) * 1024;
	word = (char *)malloc(word_size);
	word[0] = '\0';
	while (input[*i] && input[*i] != '"')
	{
		if (input[*i] == '$')
		{
			expanded = expand_var(input, i);
			if (ft_strlen(expanded) + ft_strlen(word) >= word_size - 1)
			{
				free(expanded);
				break ;
			}
			ft_strncat(word, expanded, word_size - ft_strlen(word) - 1);
			free(expanded);
		}
		else
		{
			len = ft_strlen(word);
			if (len + 1 < word_size - 1)
			{
				word[len] = input[*i];
				word[len + 1] = '\0';
			}
			(*i)++;
		}
	}
	if (input[*i] == '"')
		(*i)++;
	return (word);
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
	char	*expanded;

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
		if (input[i] == '\'')
			split.argv[j++] = handle_single_quotes(input, &i);
		else if (input[i] == '"')
		{
			split.start = ++i;
			split.argv[j++] = handle_double_quotes(input, &i);
		}
		else if (input[i] == '$')
		{
			expanded = expand_var(input, &i);
			if (expanded)
			{
				split.argv[j++] = ft_strdup(expanded);
				free(expanded);	
			}
		}
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
