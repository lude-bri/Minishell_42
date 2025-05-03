/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   205_tkns_handlers_words.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 20:00:32 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/06 20:10:10 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Handles quoted substrings within a word and performs variable expansion if needed.
 *
 * This function parses a quoted string and optionally expands environment variables
 * if the quote character is a double quote (`"`). It appends the result into a buffer.
 *
 * @param input The full input string being parsed.
 * @param qd Pointer to a t_quote structure tracking positions and buffer.
 * @param msh Pointer to the main shell state (used for env lookup).
 * @param quote The quote character used (`'` or `"`).
 */
static void	pr_quote(const char *input, t_quote *qd, t_msh *msh, char quote)
{
	char	*expanded;

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
			qd->buffer[(*qd->buf_i)++] = input[(*qd->i)++];
	}
	if (input[*qd->i] == quote)
		(*qd->i)++;
}

/**
 * @brief Parses a fully quoted word and stores it as a single token.
 *
 * Assumes the quote has already been detected and handles characters
 * up to the closing quote. Appends the word to the token array.
 *
 * @param input The input string.
 * @param sp Pointer to the token operation structure.
 * @param buffer Temporary buffer used to build the token.
 * @param buf_i Pointer to the current index in the buffer.
 */
static void	handle_quoted_word(const char *input, t_tkn_op *sp, char *buffer
														, int *buf_i)
{
	char	quote_char;

	quote_char = input[sp->i];
	sp->i++;
	while (input[sp->i] && input[sp->i] != quote_char)
		buffer[(*buf_i)++] = input[sp->i++];
	if (input[sp->i] == quote_char)
		sp->i++;
	buffer[*buf_i] = '\0';
	sp->argv[sp->j++] = ft_strdup(buffer);
}

/**
 * @brief Handles environment variable expansion within a word.
 *
 * Expands the variable (if valid) and appends its value into the parsing buffer.
 * Skips appending if it's the first word and the result is an empty string.
 *
 * @param p Pointer to a t_params struct containing context and buffers.
 */
static void	handle_variable_expansion(t_params *p)
{
	t_quote	qd;
	char	*expanded;

	qd = (t_quote){p->buffer, p->buf_i, &p->sp->i};
	expanded = expand_var(p->input, qd.i, p->msh);
	if (expanded)
	{
		if (!(p->is_first_word && *p->buf_i == 0 && expanded[0] == '\0'))
		{
			ft_strncat(qd.buffer, expanded, BUF_SIZE - *qd.buf_i - 1);
			*qd.buf_i = ft_strlen(qd.buffer);
		}
		free(expanded);
	}
}

/**
 * @brief Handles a word with no surrounding quotes.
 *
 * Parses characters until a special character or whitespace is found,
 * handling embedded quotes or variable expansions as necessary.
 *
 * @param p Pointer to a t_params struct containing parsing context and state.
 */
static void	handle_unquoted_word(t_params *p)
{
	t_quote	qd;

	while (p->input[p->sp->i] && !is_whitespace(p->input[p->sp->i])
		&& p->input[p->sp->i] != '|' && p->input[p->sp->i] != '>'
		&& p->input[p->sp->i] != '<')
	{
		if (p->input[p->sp->i] == '"' || p->input[p->sp->i] == '\'')
		{
			qd = (t_quote){p->buffer, p->buf_i, &p->sp->i};
			pr_quote(p->input, &qd, p->msh, p->input[p->sp->i]);
		}
		else if (p->input[p->sp->i] == '$')
			handle_variable_expansion(p);
		else
			p->buffer[(*p->buf_i)++] = p->input[p->sp->i++];
	}
}

/**
 * @brief Main entry point to parse a word token from the input.
 *
 * Handles both quoted and unquoted words, performs expansions,
 * and appends the final word to the token array.
 *
 * @param input The raw input string.
 * @param sp Pointer to the token operation structure.
 * @param msh Pointer to the shell state.
 */
void	handle_words(const char *input, t_tkn_op *sp, t_msh *msh)
{
	char		buffer[BUF_SIZE];
	int			buf_i;
	bool		is_first_word;
	t_params	p;

	ft_memset(buffer, 0, BUF_SIZE);
	buf_i = 0;
	is_first_word = (sp->j == 0);
	p = (t_params){input, sp, msh, buffer, &buf_i, is_first_word};
	if (is_first_word && (input[sp->i] == '"' || input[sp->i] == '\''))
	{
		handle_quoted_word(input, sp, buffer, &buf_i);
		return ;
	}
	handle_unquoted_word(&p);
	if (buf_i > 0 || !is_first_word)
	{
		buffer[buf_i] = '\0';
		sp->argv[sp->j++] = ft_strdup(buffer);
	}
}
