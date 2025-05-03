/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   560_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:48:51 by luigi             #+#    #+#             */
/*   Updated: 2024/12/23 01:50:26 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	verify_arg(char **argv, t_msh *msh, t_tkn *tokens);
int			msh_exit(char **argv, t_msh *msh, t_tkn *tokens);
static int	exit_msh(char **argv, t_msh *msh, t_tkn *tokens);

/**
 * @brief Handles `exit` when a single numeric argument is passed.
 *
 * Validates sign and digits, sets the appropriate exit status.
 *
 * @param argv Argument vector from the command line.
 * @param msh The minishell context.
 * @return Exit code to be used by the shell.
 */
static int	handle_two_commands(char **argv, t_msh *msh)
{
	if (is_letter(argv[1]))
		return (write(STDERR_FILENO, " numeric argument required\n", 27), 2);
	else
	{
		if (is_sign(argv[1]))
		{
			if (is_num(argv[1]) && ft_strncmp(argv[1], "+", 1) == 0)
				msh->exit_status = ft_atoi(argv[1]);
			else
				msh->exit_status = 256 + ft_atoi(argv[1]);
		}
		else
			return (ft_atoi(argv[1]));
	}
	return (msh->exit_status);
}

/**
 * @brief Handles `exit` when multiple arguments are passed.
 *
 * If the first argument is numeric and more follow, an error is raised:
 * `exit: too many arguments`. If non-numeric, it raises a format error.
 *
 * @param argv Argument vector.
 * @param msh The shell state context.
 * @return Exit code or error code (1/2).
 */
static int	handle_multiple_commands(char **argv, t_msh *msh)
{
	if (is_sign(argv[1]))
	{
		if (is_num(argv[2]) && ft_strncmp(argv[1], "+", 1) == 0)
			msh->exit_status = ft_atoi(argv[2]);
		else
			msh->exit_status = 256 - ft_atoi(argv[2]);
	}
	else if (is_letter(argv[1]))
	{
		msh->flag_exit = false;
		return (write(STDERR_FILENO, " numeric argument required\n", 26), 2);
	}
	else
	{
		msh->flag_exit = true;
		return (write(STDERR_FILENO, " too many arguments\n", 20), 1);
	}
	return (msh->exit_status);
}

/**
 * @brief Verifies validity of `exit` arguments and determines how to proceed.
 *
 * Handles numeric overflow, pipe conflicts, and calls the correct helper.
 *
 * @param argv Arguments passed to `exit`.
 * @param msh Shell state.
 * @param tokens The token list for cleanup.
 * @return Final exit status to use.
 */
static int	verify_arg(char **argv, t_msh *msh, t_tkn *tokens)
{
	long long	ll;

	(void)tokens;
	if (!argv || !*argv)
		msh->exit_status = exit_msh(NULL, msh, tokens);
	if (!argv[1] || ft_strcmp(argv[1], "|") == 0)
		msh->exit_status = exit_msh(argv, msh, tokens);
	if (is_num(argv[1]))
	{
		ll = ft_atoll(argv[1]);
		if (ll >= LLONG_MAX)
		{
			msh->exit_status = 2;
			return (write(STDERR_FILENO,
					" numeric argument required\n", 27), 2);
		}
	}
	if (msh->cmd_count == 2)
		return (handle_two_commands(argv, msh));
	else
		return (handle_multiple_commands(argv, msh));
}

/**
 * @brief Performs final cleanup and terminates the shell.
 *
 * Frees all relevant memory (env, tokens, command, file descriptors),
 * then calls `exit()` with the current shell status.
 *
 * @param argv Arguments passed to `exit`.
 * @param msh Shell state.
 * @param tokens The token list.
 * @return Never returns (calls `exit()` internally).
 */
static int	exit_msh(char **argv, t_msh *msh, t_tkn *tokens)
{
	if (argv != NULL)
		printf("exit\n");
	free_array(msh->envp, 0);
	free_array(msh->ex_envp, 0);
	free_msh(msh->cmds, msh, tokens);
	close(msh->fd_in);
	close(msh->fd_out);
	exit(msh->exit_status);
}

/**
 * @brief The main `exit` builtin entry point.
 *
 * Decides if the shell should exit or report an error, and finalizes the status.
 *
 * @param argv Arguments passed (e.g. `exit 1`)
 * @param msh Shell context.
 * @param tokens Token list to free if exiting.
 * @return Exit status (usually 0, 1, or 2)
 */
int	msh_exit(char **argv, t_msh *msh, t_tkn *tokens)
{
	if (msh->cmd_count >= 2)
		msh->exit_status = verify_arg(argv, msh, tokens);
	if (msh->flag_exit == false)
		msh->exit_status = exit_msh(argv, msh, tokens);
	return (msh->exit_status);
}
