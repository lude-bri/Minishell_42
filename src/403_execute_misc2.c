/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   403_execute_misc2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 17:09:35 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/12 11:42:52 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Executes the child process logic after forking.
 *
 * - Applies redirections via `exec_redirs()`
 * - Attempts to execute the command via `execute()`
 *
 * This function is meant to be called inside a forked process (pid == 0).
 *
 * @param tokens Pointer to the command token.
 * @param msh Pointer to the shell state.
 */
void	handle_child_process(t_tkn *tokens, t_msh *msh)
{
	exec_redirs(tokens, msh);
	execute(msh, tokens);
}

/**
 * @brief Handles the parent process logic after forking a child.
 *
 * - Restores signal handling.
 * - Waits for the child to finish.
 * - Updates `msh->exit_status` depending on exit or signal.
 *
 * @param msh Pointer to the shell state.
 * @param status Status variable to receive `waitpid()` output.
 */
void	handle_parent_process(t_msh *msh, int status)
{
	setup_signals();
	waitpid(msh->pid, &status, 0);
	if (WIFEXITED(status))
		msh->exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		msh->exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
	}
}

/**
 * @brief Handles the "command not found" error.
 *
 * - Prints error message to stderr.
 * - Frees allocated memory.
 * - Exits with code 127 (POSIX standard for command not found).
 *
 * @param msh Pointer to the shell state.
 * @param tokens Pointer to the token list.
 * @param args Argument vector passed to `execve()`.
 */
void	handle_command_not_found(t_msh *msh, t_tkn *tokens, char **args)
{
	write(STDERR_FILENO, " command not found\n", 19);
	free_msh(msh->cmds, msh, tokens);
	free_array(msh->envp, 0);
	free_arg(msh->ex_envp);
	free_arg(args);
	exit(127);
}

/**
 * @brief Handles an `execve()` failure.
 *
 * - Prints system error message via `perror(path)`.
 * - Frees memory.
 * - Exits with code 126 (POSIX: command found but not executable).
 *
 * @param path The path that failed to execute.
 * @param args Argument vector passed to `execve()`.
 * @param msh Pointer to the shell state.
 * @param tokens Pointer to the token list.
 */
void	handle_execve_error(char *path, char **args, t_msh *msh, t_tkn *tokens)
{
	perror(path);
	free(path);
	free_arg(args);
	free_msh(msh->cmds, msh, tokens);
	free_array(msh->envp, 0);
	free_arg(msh->ex_envp);
	exit(126);
}
