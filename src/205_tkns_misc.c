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

/**
 * @brief Advances the index past a quoted string segment.
 *
 * Skips over characters inside a quote (`'...'` or `"..."`), including the
 * closing quote if present. Used to ignore quoted segments when counting or parsing.
 *
 * @param input The full input string.
 * @param i The index where the quote starts.
 * @return The index right after the closing quote.
 */
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

/**
 * @brief Checks whether the current character is a shell-special character.
 *
 * Specifically looks for pipe (`|`), redirections (`<`, `>`), or dollar (`$`).
 * Advances the index if a match is found.
 *
 * @param input The input string.
 * @param i Pointer to the current index; will be incremented if a match is found.
 * @return true if a special character was found; false otherwise.
 */
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

/**
 * @brief Advances the index through regular characters (non-special, non-quoted).
 *
 * Stops when a quote, special character, or whitespace is found.
 *
 * @param input The input string.
 * @param i Pointer to the current index; will be incremented through valid characters.
 * @return true if at least one character was consumed; false otherwise.
 */
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

/**
 * @brief Appends a single character from the input to the current word buffer.
 *
 * Ensures that the buffer has space before appending, and then adds a null-terminator.
 *
 * @param input The input string.
 * @param i Pointer to the current index in the input string; will be incremented.
 * @param msh Pointer to the shell state (contains the word buffer and size).
 */
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

/**
 * @brief Calculates the logical length of the input string for token parsing.
 *
 * Skips initial whitespace, then returns the number of characters in the rest of the string.
 * Used to determine allocation size for tokens (e.g., in `split_input`).
 *
 * @param input The input string.
 * @return The number of characters after leading whitespace.
 */
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
