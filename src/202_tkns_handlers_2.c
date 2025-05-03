/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   202_tkns_handlers_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 23:13:00 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/06 19:25:21 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Handles parsing of a double-quoted string, with environment variable expansion.
 *
 * Allocates a large buffer for the quoted string, then iterates through the input,
 * appending characters and expanding variables (e.g., "$USER") as needed.
 * Stops at the closing double quote.
 *
 * @param input The raw input string.
 * @param i Pointer to the current index, which is incremented during parsing.
 * @param msh Pointer to the shell state structure, used for buffer storage and env access.
 * @return A newly allocated string with the processed content, or NULL on allocation failure.
 */
char	*handle_double_quotes(const char *input, int *i, t_msh *msh)
{
	msh->word_size = ft_strlen(input) * 1024;
	msh->word = (char *)malloc(msh->word_size);
	if (!msh->word)
		return (NULL);
	msh->word[0] = '\0';
	while (input[*i] && input[*i] != '"')
	{
		if (input[*i] == '$' && input[*i + 1] && input[*i + 1] != ' '
			&& input[*i + 1] != '"')
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

/**
 * @brief Handles parsing of a single-quoted string (no variable expansion).
 *
 * Copies characters between single quotes `'...'` as a literal string.
 *
 * @param input The raw input string.
 * @param i Pointer to the current index, which is incremented during parsing.
 * @return A newly allocated string containing the content between single quotes.
 */
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

/**
 * @brief Handles the heredoc redirection token ('<<').
 *
 * Adds the token `"<<"` to the token array, optionally quoted
 * as `"\"<<\""` if surrounded by double quotes in the original input.
 *
 * @param input The raw input string.
 * @param sp Pointer to the token operation structure.
 */
void	handle_double_in_redir(const char *input, t_tkn_op *sp)
{
	if (sp->i > 0 && input[sp->i - 1] == '"')
		sp->argv[(sp->j)++] = ft_strdup("\"<<\"");
	else
		sp->argv[(sp->j)++] = ft_strdup("<<");
	(sp->i) += 2;
}

/**
 * @brief Handles the single input redirection token ('<').
 *
 * Considers special edge cases when the command line is very short
 * or the token is surrounded by null characters or quotes.
 * Appends the `<` token to the token list accordingly.
 *
 * @param input The raw input string.
 * @param sp Pointer to the token operation structure.
 * @param msh Pointer to the main shell state structure.
 */
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
