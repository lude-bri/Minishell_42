/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   500_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:48:34 by luigi             #+#    #+#             */
/*   Updated: 2024/12/23 00:27:21 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Checks whether there are whitespaces after the `echo` keyword.
 *
 * Used to determine if spacing is needed between arguments when printing.
 *
 * @param line Full input line from the user.
 * @return SUCCESS if whitespace is found; FAILURE otherwise.
 */
static int	verify_whitespaces(char *line)
{
	int		i;

	if (ft_strncmp("echo ", line, 5) == 0)
	{
		i = 5;
		while (line[i])
		{
			if (is_whitespace(line[i]))
				return (SUCCESS);
			i++;
		}
		return (FAILURE);
	}
	return (FAILURE);
}

/**
 * @brief Parses `-n` flags in echo arguments.
 *
 * Recognizes multiple `-n` flags like `-n`, `-nn`, `-nnn`, etc.
 * If valid `-n` options are found, disables trailing newline output.
 *
 * @param argv Argument array.
 * @param newline Pointer to the newline flag (set to 0 if `-n` is found).
 * @return Index of the first non-option argument.
 */
int	handle_echo_options(char **argv, int *newline)
{
	int		i;
	int		j;

	*newline = 1;
	i = 1;
	while (argv[i] && argv[i][0] == '-' && argv[i][1] == 'n')
	{
		j = 2;
		while (argv[i][j] && argv[i][j] == 'n')
			j++;
		if (argv[i][j] != '\0')
			break ;
		*newline = 0;
		i++;
	}
	return (i);
}

/**
 * @brief Prints the echo arguments starting from index `i`.
 *
 * Adds a space between arguments if `space` is true.
 *
 * @param argv Argument array.
 * @param i Starting index to print from.
 * @param space Whether to print spaces between arguments.
 */
void	print_echo_arguments(char **argv, int i, int space)
{
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1] && space == 1)
			printf(" ");
		i++;
	}
}

/**
 * @brief Validates that all quote characters are properly closed.
 *
 * Ensures that quotes come in pairs. Used to detect malformed input like `echo "abc`.
 *
 * @param str The input line.
 * @return SUCCESS if unbalanced quotes are found (i.e., invalid); FAILURE if all quotes are balanced.
 */
static int	verify_sanity_quotes(char *str)
{
	int		found;
	int		i;

	i = 0;
	found = 0;
	while (str[i])
	{
		if (str[i] == '\"' || str[i] == '\'')
		{
			if (found == 1)
				found = 0;
			else
				found = 1;
		}
		i++;
	}
	if (found == 1)
		return (SUCCESS);
	else
		return (FAILURE);
}

/**
 * @brief Executes the `echo` builtin logic.
 *
 * - Handles `-n` options
 * - Prints arguments with optional spacing
 * - Checks for malformed quotes and prints error if found
 *
 * @param argv Argument array (may include path or previous tokens).
 * @param msh Pointer to the shell state (used for line + status).
 * @param tokens Unused in this implementation.
 * @return Always returns 0.
 */
int	msh_echo(char **argv, t_msh *msh, t_tkn *tokens)
{
	int	i;
	int	newline;
	int	space;

	(void)tokens;
	while (ft_strcmp(*argv, "echo") != 0)
		argv++;
	if (!argv[1])
		return (printf("\n"), 0);
	i = handle_echo_options(argv, &newline);
	if (verify_sanity_quotes(msh->line) == SUCCESS)
	{
		write(2, "msh: unexpected EOF while looking for matching quotes\n", 54);
		msh->exit_status = 2;
		return (0);
	}
	if (verify_whitespaces(msh->line) == SUCCESS)
		space = 1;
	else
		space = 0;
	print_echo_arguments(argv, i, space);
	if (newline)
		printf("\n");
	return (0);
}
