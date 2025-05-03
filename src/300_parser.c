/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   300_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:28:36 by luigi             #+#    #+#             */
/*   Updated: 2025/01/08 17:12:59 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Counts the number of pipe segments in a token list.
 *
 * Starts from 1 if at least one token exists, and increments the count
 * for each `TKN_PIPE` token encountered.
 *
 * @param tokens Pointer to the head of the token list.
 * @return Number of pipeline segments (commands separated by pipes).
 */
int	count_pipes(t_tkn *tokens)
{
	int		counter;

	counter = 0;
	while (tokens)
	{
		if (!counter)
			counter = 1;
		if (tokens->type == TKN_PIPE)
			counter++;
		tokens = tokens->next;
	}
	return (counter);
}

/**
 * @brief Displays a syntax error message based on the first token.
 *
 * If the first argument is a pipe (`|`), prints an error about an unexpected pipe.
 * Otherwise, prints an error about an unexpected newline.
 *
 * Sets `msh->exit_status` to 2 (standard shell syntax error code).
 *
 * @param msh Pointer to the shell state structure.
 */
void	error_syntax(t_msh *msh)
{
	if (*msh->cmds->av[0] == '|')
		printf("msh: syntax error near unexpected token `|'\n");
	else
		printf("msh: syntax error near unexpected token `newline'\n");
	msh->exit_status = 2;
}

/**
 * @brief Checks whether a string represents a shell operator.
 *
 * Returns SUCCESS if the first character is one of: `|`, `<`, `>`.
 * Otherwise, returns FAILURE.
 *
 * @param line The input string to check.
 * @return SUCCESS (1) if operator; FAILURE (0) otherwise.
 */
int	is_operator(char *line)
{
	if (ft_strcmp(line, "") == 0)
		return (FAILURE);
	if (*line == '\0')
		return (FAILURE);
	if (*line == '|' || *line == '<' || *line == '>')
		return (SUCCESS);
	return (FAILURE);
}

/**
 * @brief Runs high-level syntax validation on the current input line.
 *
 * Uses two helper functions to detect common shell syntax errors.
 * If any check fails, sets appropriate exit code and returns FAILURE.
 *
 * @param msh Pointer to the shell state.
 * @param line The raw input string.
 * @return SUCCESS if valid; FAILURE if syntax errors are detected.
 */
int	syntax(t_msh *msh, char *line)
{
	char	*str;

	str = *msh->cmds->av;
	if (initial_syntax_checks(msh, line, str) == FAILURE)
		return (FAILURE);
	if (detailed_syntax_checks(msh, str, line) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}

/**
 * @brief Parses the user input into tokens after basic validation.
 *
 * - Prompts the user for input and stores the line.
 * - Splits the line into arguments.
 * - Validates syntax.
 * - Converts arguments into token structures.
 * - Stores command and pipe counts in the shell state.
 *
 * @param msh Pointer to the shell state structure.
 * @return Pointer to the head of the token list, or NULL on error.
 */
t_tkn	*to_parse(t_msh *msh)
{
	t_tkn	*tokens;
	char	*line;

	line = build_prompt(msh);
	if (!line)
		return (NULL);
	add_history(line);
	msh->cmds = ft_calloc(1, sizeof(t_command));
	if (!msh->cmds)
		return (NULL);
	msh->cmds->av = split_input(line, msh);
	if (!msh->cmds->av || !*msh->cmds->av)
		return (free_parse(msh), NULL);
	if (syntax(msh, line) == FAILURE)
		return (free_arg(msh->cmds->av), free(msh->cmds), NULL);
	msh->cmd_count = ft_matrixlen(msh->cmds->av);
	msh->line = ft_strdup(line);
	tokens = tokenizer(msh, msh->cmds->av);
	msh->pipe_count = count_pipes(tokens);
	if (line)
	{
		free(line);
		line = NULL;
	}
	return (tokens);
}
