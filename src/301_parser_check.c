/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   301_parser_check.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 15:43:11 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/07 16:57:37 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Performs basic checks on the beginning of the input line.
 *
 * Validates whether the input starts with valid redirection operators (`<<`, `>>`, `<`),
 * and ensures they are not used in invalid or incomplete contexts.
 *
 * Calls `error_syntax()` and sets `exit_status = 2` when necessary.
 *
 * @param msh Pointer to the shell state.
 * @param line The full input line to analyze.
 * @return SUCCESS if syntax is valid; FAILURE otherwise.
 */
int	check_initial_syntax(t_msh *msh, char *line)
{
	if ((ft_strncmp(line, "<<", 2) == 0) || ft_strncmp(line, ">>", 2) == 0)
	{
		if (ft_strlen(line) >= 3)
			return (SUCCESS);
		else
			return (error_syntax(msh), FAILURE);
	}
	else if (ft_strncmp(line, "<", 1) == 0)
	{
		if (ft_strlen(line) > 1)
			return (SUCCESS);
	}
	else if ((ft_strncmp(line, "|", 1) == 0)
		|| (ft_strncmp(line, "<", 1) == 0)
		|| (ft_strncmp(line, ">", 1) == 0))
		return (error_syntax(msh), FAILURE);
	return (SUCCESS);
}

/**
 * @brief Checks whether an operator appears at the end of the string.
 *
 * Ensures that characters like `|`, `>`, `<` are not the final character,
 * which would indicate a syntax error.
 *
 * @param msh Pointer to the shell state.
 * @param str A substring of the parsed command.
 * @return SUCCESS if valid; FAILURE otherwise.
 */
int	check_str_syntax(t_msh *msh, char *str)
{
	int	i;

	i = 0;
	while (str[i] != '\0')
	{
		if (i > 0 && (str[0] == '<' && str[1] == '<'))
			return (SUCCESS);
		if (str[i] == '|' || str[i] == '>' || str[i] == '<')
		{
			if (str[i + 1] == '\0')
				return (error_syntax(msh), FAILURE);
		}
		i++;
	}
	return (SUCCESS);
}

/* int	check_line_syntax(t_msh *msh, char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if ((ft_strncmp(line, "echo \"", 6) == 0)
			|| ft_strncmp(line, "echo \'", 6) == 0)
			return (SUCCESS);
		if (is_operator(&line[i]) == FAILURE)
		{
			i++;
			continue ;
		}
		if ((ft_strncmp(&line[i], "<<", 2) == 0)
			|| (ft_strncmp(&line[i], ">>", 2) == 0))
		{
			i += 2;
			if (is_operator(&line[i]) == SUCCESS)
				return (error_syntax(msh), FAILURE);
		}
		if (line[i] && line[i + 1])
		{
			if (is_operator(&line[i + 1]) == SUCCESS)
				return (error_syntax(msh), FAILURE);
			i++;
		}
	}
	return (SUCCESS);
} */

/**
 * @brief Runs a series of initial syntax checks on the input.
 *
 * Delegates to helper functions for detailed verification:
 * - Verifies the start of the input.
 * - Checks for malformed operators.
 * - Detects forbidden patterns such as `||`, `>>>`, `><`, etc.
 *
 * @param msh Pointer to the shell state.
 * @param line Full input line.
 * @param str Parsed first token (usually the command).
 * @return SUCCESS if input is syntactically correct; FAILURE otherwise.
 */
int	initial_syntax_checks(t_msh *msh, char *line, char *str)
{
	int	len;

	len = ft_strlen(line);
	if (check_initial_syntax(msh, line) == FAILURE)
		return (FAILURE);
	if (check_str_syntax(msh, line) == FAILURE)
		return (FAILURE);
	if (check_line_syntax(msh, line) == FAILURE)
		return (FAILURE);
	if (ft_strnstr(str, "||", len) || ft_strnstr(str, "&&", len)
		|| ft_strnstr(str, "| |", len) || ft_strnstr(line, ">>>", len)
		|| ft_strnstr(str, "<>", len) || ft_strnstr(str, "><", len)
		|| ft_strnstr(str, "?>", len) || ft_strnstr(str, ">?", len))
		return (error_syntax(msh), FAILURE);
	return (SUCCESS);
}

/**
 * @brief Performs stricter and more contextual syntax checks.
 *
 * Ensures that operators like `>`, `<`, `|`, `&` are followed by valid tokens,
 * and not by space, null terminator, or more operators.
 *
 * Handles some heredoc cases (`<<`) as valid.
 *
 * @param msh Pointer to the shell state.
 * @param str Parsed command string.
 * @param line Original input line.
 * @return SUCCESS if syntax is valid; FAILURE otherwise.
 */
int	detailed_syntax_checks(t_msh *msh, char *str, char *line)
{
	int	i;

	i = 0;
	if (str[i] == '<')
	{
		if (str[i + 1] != '\0' && str[i + 1] == '<')
			return (SUCCESS);
	}
	if (line[i] == '<')
	{
		if (line[i + 1] != '\0' && ft_strlen(line) > 1)
			return (SUCCESS);
	}
	while (str[i] != '\0')
	{
		if ((str[i] == '&' || str[i] == '|' || str[i] == '>' || str[i] == '<'))
		{
			if (str[i + 1] == '\0' || str[i + 1] == ' '
				|| str[i + 1] == '&' || str[i + 1] == '|')
				return (error_syntax(msh), FAILURE);
		}
		i++;
	}
	return (SUCCESS);
}
