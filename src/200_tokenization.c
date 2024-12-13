/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   200_tokenization.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:49:13 by luigi             #+#    #+#             */
/*   Updated: 2024/12/09 17:43:26 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// static char	**free_arrays(const char **s, int a);

char	*expand_var(const char *input, int *i, t_msh *msh)
{
	int		len;
	int		start;
	char	*var_name;
	char	*value;
	char	*result;

	len = 0;
	start = *i + 1;
	if (!input[1])
		return (NULL);
	if (input[start] == '?')
	{
		// printf("Current msh->exit_status before expanding $?: %d\n", msh->exit_status);
		result = ft_itoa(msh->exit_status);
		// printf("Expanding $?: %s\n", result);
		*i += 2;
		return (result); 
	}
	while (input[start + len] && (ft_isalnum(input[start + len]) || input[start + len] == '_'))
		len++;
	var_name = (char *)malloc(len + 1);
	if (!var_name)
		return (NULL);
	ft_strlcpy(var_name, &input[start], len + 1);
	var_name[len] = '\0';
	// value = getenv(var_name);
	value = get_variable(var_name, msh->envp);
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

	len = end - start;
	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = input[start + i];
		i++;
	}
	word[len] = '\0';
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
		if (input[i] == '\'' || input[i] == '"')
		{
			i++;
			while (input[i] && (input[i] != '\'' && input[i] != '"'))
				i++;
			if (!input[i])
				break ;
			i++;
			counter++;
		}
		else if (input[i] == '|')
		{
			counter++;
			i++;
		}
		else if (input[i] == '>' || input[i] == '<')
		{
			counter++;
			i++;
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
		else if (input[i] == '$')
		{
			i++;
			counter++;
		}
		else if (input[i])
		{
			counter++;
			while (input[i] && input[i] != '\'' && input[i] != '"' && input[i] != '|'
				&& input[i] != '<' && input[i] != '>' && !is_whitespace(input[i]))
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

char	*handle_double_quotes(const char *input, int *i, t_msh *msh)
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
		if (input[*i] == '$' && input[*i + 2] != '\0')
		{
			expanded = expand_var(input, i, msh);
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

char	**split_input(const char *input, t_msh *msh)
{
	t_tkn_op	split;
	int			i;
	int			j;
	char		*expanded;

	split.number_words = count_words(input);
	printf("Token count: %d\n", split.number_words);
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
			if (input[i] == '"')
				split.argv[j++] = ft_strdup("");
			else if ((ft_strncmp(input, "|", 1) != 0) && (ft_strncmp(input, ">", 1) != 0)
				&& (ft_strncmp(input, "<", 1) != 0) && ft_strncmp(input, "<<", 2) != 0)
				split.argv[j++] = handle_double_quotes(input, &i, msh);	
		}
		else if (input[i] == '$')
		{
			if (input[i + 1] == '>') //verificar todos os outros operadores
			{
				syntax_check_redirs(msh, NULL);
				break ;
			}
			if (input[i + 1] == '\0' || input[i + 1] == '\"')
			{
				split.argv[j++] = ft_strdup("$");
				i++;
			}
			else
			{
				expanded = expand_var(input, &i, msh);
				if (expanded)
				{
					split.argv[j++] = ft_strdup(expanded);
					free(expanded);	
				}
			}
		}
		else if (input[i] == '|')
		{
			if (input[i - 1] == '"' || input[i + 1] == '"')
				split.argv[j++] = ft_strdup("\"|\"");
			else
				split.argv[j++] = ft_strdup("|");
			i++;
		}	
		else if (input[i] == '<' && input[i + 1] == '<')
		{
			if (input[i - 1] == '"')
				split.argv[j++] = ft_strdup("\"<<\"");
			else
				split.argv[j++] = ft_strdup("<<");
			i += 2;
		}
		else if (input[i] == '<')
		{
			if (input[i - 1] == '"' || input[i + 1] == '"')
				split.argv[j++] = ft_strdup("\"<\"");
			else
				split.argv[j++] = ft_strdup("<");
			i++;
		}
		else if (input[i] == '>')
		{
			if (input[i - 1] == '"' || input[i + 1] == '"')
				split.argv[j++] = ft_strdup("\">\"");
			else
				split.argv[j++] = ft_strdup(">");
			i++;
		}
		else if (input[i] && !is_whitespace(input[i]))
		{
			split.start = i;
			while (input[i] && input[i] != '\'' && input[i] != '"' && input[i] != '|' && input[i] != '>' 
				&& input[i] != '<' && !is_whitespace(input[i]))
				i++;
			split.argv[j++] = copy_word(input, split.start, i);
		}	
		if (!split.argv[j - 1])
		{
			printf("Error: Memory allocation failed for token %d\n", j - 1);
			while (--j >= 0)
				free(split.argv[j]);
			free(split.argv);
			return (NULL);
		}
	}
	split.argv[j] = NULL;
	i = 0;
	while (i < j)
	{
		printf("Token %d: %s\n", i, split.argv[i]);
		i++;
	}
	return (split.argv);
}

