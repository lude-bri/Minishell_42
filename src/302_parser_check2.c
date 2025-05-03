/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   302_parser_check2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 16:58:16 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/07 16:59:53 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Checks if the input line starts with a properly quoted `echo` command.
 *
 * This is used como um caso especial para permitir `echo "..."` ou `echo '...'`
 * mesmo quando há operadores que normalmente causariam erro de sintaxe.
 *
 * @param line The raw input line.
 * @return SUCCESS if `echo` is quoted properly; FAILURE otherwise.
 */
int	check_echo_command(char *line)
{
	if ((ft_strncmp(line, "echo \"", 6) == 0)
		|| ft_strncmp(line, "echo \'", 6) == 0)
		return (SUCCESS);
	return (FAILURE);
}

/**
 * @brief Verifica a validade da sequência de operadores em uma linha.
 *
 * Permite apenas operadores válidos e impede casos como `<<<`, `>>|`, `>|`, etc.
 * Também avança o índice da linha conforme consome os tokens.
 *
 * @param msh Pointer to the shell state.
 * @param line The raw input line.
 * @param i Pointer to the current index within the line.
 * @return SUCCESS if the current operator syntax is valid; FAILURE otherwise.
 */
int	check_operators(t_msh *msh, char *line, int *i)
{
	if (is_operator(&line[*i]) == FAILURE)
	{
		(*i)++;
		return (SUCCESS);
	}
	if ((ft_strncmp(&line[*i], "<<", 2) == 0)
		|| (ft_strncmp(&line[*i], ">>", 2) == 0))
	{
		*i += 2;
		if (is_operator(&line[*i]) == SUCCESS)
			return (error_syntax(msh), FAILURE);
	}
	if (line[*i] && line[*i + 1])
	{
		if (is_operator(&line[*i + 1]) == SUCCESS)
			return (error_syntax(msh), FAILURE);
		(*i)++;
	}
	return (SUCCESS);
}

/**
 * @brief Performs a final pass to validate the full input line's operator logic.
 *
 * - Bypasses further checks if the command is a quoted `echo`.
 * - Iterates through the input line and validates operator usage via `check_operators()`.
 *
 * @param msh Pointer to the shell state.
 * @param line The full user input line.
 * @return SUCCESS if line syntax is valid; FAILURE otherwise.
 */
int	check_line_syntax(t_msh *msh, char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (check_echo_command(line) == SUCCESS)
			return (SUCCESS);
		if (check_operators(msh, line, &i) == FAILURE)
			return (FAILURE);
	}
	return (SUCCESS);
}
