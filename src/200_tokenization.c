/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   200_tokenization.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:49:13 by luigi             #+#    #+#             */
/*   Updated: 2025/01/07 16:45:25 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Copies a substring from input based on start and end indexes.
 *
 * Allocates and returns a null-terminated string containing characters from
 * input[start] up to input[end - 1].
 *
 * @param input The original input string.
 * @param start The start index of the word to copy.
 * @param end The end index (non-inclusive).
 * @return A newly allocated string containing the extracted word, or NULL on failure.
 */
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


/**
 * @brief Counts the number of tokens (words, operators, quoted strings) in the input.
 *
 * Iterates through the input string, using helper functions to handle quotes,
 * special characters (like |, <, >), and regular words.
 *
 * @param input The original input string.
 * @return The total number of recognized tokens.
 */
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
			i = quote_helper(input, i);
			counter++;
		}
		else if (special_char_helper(input, &i))
			counter++;
		else if (regular_char_helper(input, &i))
			counter++;
	}
	return (counter);
}

/**
 * @brief Tokenizes the input string and fills the token buffer.
 *
 * Walks through the input, handling different types of tokens (quotes, expansions,
 * redirections, pipes, regular words). Allocates strings dynamically.
 * On memory allocation failure, prints an error and frees the partially filled token list.
 *
 * @param input The input string to tokenize.
 * @param sp Pointer to a t_tkn_op structure that tracks token position and storage.
 * @param msh Pointer to the main shell state structure.
 */
void	process_tokens(const char *input, t_tkn_op *sp, t_msh *msh)
{
	while (input[sp->i])
	{
		while (input[sp->i] && is_whitespace(input[sp->i]))
			sp->i++;
		if (input[sp->i] == '\'' || input[sp->i] == '"')
			handle_quotes(input, sp, msh);
		else if (input[sp->i] == '$')
			handle_expand(input, sp, msh);
		else if (input[sp->i] == '|')
			handle_pipes(input, sp);
		else if (input[sp->i] == '<')
			handle_input_redir(input, sp, msh);
		else if (input[sp->i] == '>')
			handle_output_redir(input, sp, msh);
		else if (input[sp->i] && !is_whitespace(input[sp->i]))
			handle_words(input, sp, msh);
		if (sp->j > 0 && !sp->argv[sp->j - 1])
		{
			printf("Error: Memory allocation failed for token %d\n", sp->j - 1);
			while (--sp->j >= 0)
				free(sp->argv[sp->j]);
			free(sp->argv);
			return ;
		}
	}
}

/**
 * @brief Entry point for tokenizing the input.
 *
 * Skips initial whitespace and calls process_tokens() to fill the token array.
 *
 * @param input The raw input string to parse.
 * @param sp Pointer to the token operation structure.
 * @param msh Pointer to the shell state structure.
 */
void	process_input(const char *input, t_tkn_op *sp, t_msh *msh)
{
	skip_whitespace(&input);
	process_tokens(input, sp, msh);
}

/**
 * @brief Splits the input string into tokens and returns them as an array of strings.
 *
 * Allocates a dynamic array of strings (argv-style), fills it by parsing input,
 * and ensures NULL-termination.
 *
 * @param input The command-line input string.
 * @param msh Pointer to the shell state structure, used to calculate token length.
 * @return A NULL-terminated array of strings representing the tokens, or NULL on failure.
 */
char	**split_input(const char *input, t_msh *msh)
{
	t_tkn_op	sp;

	sp.i = 0;
	sp.j = 0;
	msh->len = take_len(input);
	sp.argv = (char **)malloc(sizeof(char *) * (msh->len + 1));
	if (!sp.argv)
		return (NULL);
	process_input(input, &sp, msh);
	sp.argv[sp.j] = NULL;
	return (sp.argv);
}
