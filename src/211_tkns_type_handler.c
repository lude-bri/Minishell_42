/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   211_tkns_type_handler.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 15:01:31 by mde-agui          #+#    #+#             */
/*   Updated: 2024/12/24 12:39:31 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Verifies if a given operator is adjacent to any quote character.
 *
 * Checks whether the character immediately before or after the operator
 * is a single or double quote. Used to determine whether a token
 * should be treated literally or as a special shell operator.
 *
 * @param operator The operator character to check (e.g., '<', '>', '|').
 * @param line The full line where the operator was parsed from.
 * @return SUCCESS (1) if near quotes; FAILURE (0) otherwise.
 */
int	verify_quotes(char operator, char *line)
{
	int		i;

	i = 0;
	while (line[i] != operator)
		i++;
	if (i > 0)
	{
		if (line[i + 1] == '"' || line[i - 1] == '"'
			|| line[i + 1] == '\'' || line[i - 1] == '\'')
			return (SUCCESS);
	}
	return (FAILURE);
}

/**
 * @brief Checks if an operator is surrounded on both sides by the same type of quote.
 *
 * Useful to detect when something like `"|"` or `'<'` should be treated
 * as a literal string rather than a shell operator.
 *
 * @param operator The operator character to evaluate.
 * @param line The original input line.
 * @return SUCCESS (1) if both sides are quoted; FAILURE (0) otherwise.
 */
int	verify_inbetween_quotes(char operator, char *line)
{
	int		i;

	i = 0;
	while (line[i] != operator)
		i++;
	if (i > 0)
	{
		if ((line[i + 1] == '"' && line[i - 1] == '"')
			|| (line[i + 1] == '\'' && line[i - 1] == '\''))
			return (SUCCESS);
	}
	return (FAILURE);
}

/**
 * @brief Assigns the appropriate type for a token that might represent a heredoc (`<<`).
 *
 * If the heredoc operator is quoted in the input line, it's treated as a command (literal).
 * Otherwise, the token is marked as `TKN_HEREDOC`.
 *
 * @param token Pointer to the token being evaluated.
 * @param line The original input line where the token appeared.
 */
void	handle_heredoc_token(t_tkn *token, char *line)
{
	if (verify_quotes(token->name[0], line) == FAILURE)
		token->type = TKN_HEREDOC;
	else
		token->type = TKN_CMD;
}

/**
 * @brief Determines and assigns the correct type for various shell tokens.
 *
 * Checks whether the token represents a pipe, redirection, or heredoc, and
 * assigns the appropriate type (e.g., `TKN_PIPE`, `TKN_APPEND`, `TKN_IN`, etc.).
 * If none match, defaults to `TKN_CMD`.
 *
 * Handles quoted operators correctly (e.g., `"|"` becomes `TKN_CMD`).
 *
 * @param tkn Pointer to the token being evaluated.
 * @param av Pointer to the current argument in the token list.
 * @param msh Pointer to the main shell structure (used to access the raw input line).
 */
void	handle_other_tokens(t_tkn *tkn, char **av, t_msh *msh)
{
	char	*line;

	line = msh->line;
	if (tkn->name[0] == '|')
		handle_pipe_token(tkn, line);
	else if (*av[0] == '>' && av[1] != NULL)
	{
		if (ft_strncmp(*av, ">>", 2) == 0)
			handle_append_token(tkn, line);
		else
			handle_out_token(tkn, line);
	}
	else if (tkn->name[0] == '<' && tkn->name[1] == '<' && tkn->name[2] != '\"')
		handle_heredoc_token(tkn, line);
	else if (tkn->name[0] == '<' && av[1] != NULL)
		handle_in_token(tkn, line);
	else if (tkn->name[0] == '>')
	{
		if (verify_quotes(tkn->name[0], line) == FAILURE)
			handle_out_token(tkn, line);
		else
			tkn->type = TKN_CMD;
	}
	else
		tkn->type = TKN_CMD;
}
