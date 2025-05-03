/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   520_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:49:08 by luigi             #+#    #+#             */
/*   Updated: 2024/12/23 00:37:45 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Checks for invalid options passed to the `pwd` command.
 *
 * Skips over any tokens until it finds the actual `pwd` keyword,
 * then checks if the following argument begins with `-`, which is
 * not supported in this implementation.
 *
 * @param av The full argument vector (may contain previous tokens).
 * @return 1 if an invalid option is found; 0 otherwise.
 */
static int	verify_pwd_args(char **av)
{
	while (ft_strcmp(*av, "pwd") != 0)
		av++;
	av++;
	if (ft_strncmp(*av, "-", 1) == 0)
	{
		write(2, "msh: invalid option\n", 21);
		return (1);
	}
	return (0);
}

/**
 * @brief Executes the `pwd` builtin command.
 *
 * - Calls `getcwd()` to retrieve the current working directory.
 * - Prints the path to stdout.
 * - Validates arguments to reject unsupported options (e.g., `pwd -x`).
 * - Handles memory allocation and error reporting.
 *
 * @param msh Pointer to the shell state.
 * @param tokens Pointer to the current command token (unused here).
 * @return Always returns 1 (whether successful or failed).
 */
int	msh_pwd(t_msh *msh, t_tkn *tokens)
{
	char	*cwd;
	int		found;

	(void)tokens;
	found = 0;
	if (msh->cmds->av[1])
		found = verify_pwd_args(msh->cmds->av);
	if (found == 1)
		return (1);
	cwd = getcwd(NULL, 0);
	if (cwd != NULL)
	{
		write(STDOUT_FILENO, cwd, ft_strlen(cwd));
		write(STDOUT_FILENO, "\n", 1);
		free(cwd);
		return (1);
	}
	else
	{
		perror("pwd");
		return (1);
	}
	return (0);
}
