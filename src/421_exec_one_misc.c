/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   421_exec_one_misc.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 11:48:29 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/12 12:01:33 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Executes a builtin command in a forked child process.
 *
 * - Calls `exec_bi()` to run the builtin logic.
 * - Cleans up memory and file descriptors via `free_and_close_all()`.
 * - Exits with the current shell exit status.
 *
 * This function is called inside a child process created by `handle_builtin_commands()`.
 *
 * @param msh Pointer to the shell state.
 * @param tokens Pointer to the command token.
 */
void	handle_child_builtin(t_msh *msh, t_tkn *tokens)
{
	exec_bi(tokens, msh);
	free_and_close_all(msh, tokens);
	exit(msh->exit_status);
}

/**
 * @brief Waits for a builtin child process and sets the exit status.
 *
 * - Waits for the child process to complete using `waitpid`.
 * - Updates `msh->exit_status` based on exit code or signal.
 * - Prints a newline if the process was interrupted by `SIGINT`.
 *
 * This function is called from the parent after forking a builtin.
 *
 * @param pid PID of the child process.
 * @param msh Pointer to the shell state.
 */
void	handle_parent_builtin(int pid, t_msh *msh)
{
	int	status;

	setup_signals();
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		msh->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		msh->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
	}
}
