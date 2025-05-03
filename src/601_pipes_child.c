/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   601_pipes_child.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 02:03:30 by mde-agui          #+#    #+#             */
/*   Updated: 2024/12/23 03:21:11 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Executes the left side of a pipe (`|`) in a child process.
 *
 * - Closes the unused read-end of the pipe.
 * - Redirects STDOUT to the pipe write-end using `dup2`.
 * - Calls `exec_more()` recursively for left-side execution.
 *
 * Cleans up allocated memory and exits with the current shell status.
 *
 * @param tokens The current pipe token node.
 * @param msh The shell context.
 * @param fd Pipe file descriptor array [2].
 */
void	handle_left_child(t_tkn *tokens, t_msh *msh, int *fd)
{
	close(fd[0]);
	apply_pipe(tokens->left, msh, fd, 1);
	free_arg(msh->envp);
	free_arg(msh->ex_envp);
	free_msh(msh->cmds, msh, tokens->left);
	exit(msh->exit_status);
}

/**
 * @brief Executes the right side of a pipe (`|`) in a child process.
 *
 * - Closes the unused write-end of the pipe.
 * - Redirects STDIN to the pipe read-end using `dup2`.
 * - Calls `exec_more()` recursively for right-side execution.
 *
 * Cleans up memory and exits with the current shell status.
 *
 * @param tokens The current pipe token node.
 * @param msh The shell context.
 * @param fd Pipe file descriptor array [2].
 */
void	handle_right_child(t_tkn *tokens, t_msh *msh, int *fd)
{
	close(fd[1]);
	apply_pipe(tokens->right, msh, fd, 0);
	free_arg(msh->envp);
	free_arg(msh->ex_envp);
	free_msh(msh->cmds, msh, tokens->left);
	exit(msh->exit_status);
}

/**
 * @brief Waits for both child processes created for pipe execution.
 *
 * - Waits on `pid_left` and `pid_right`.
 * - Updates `msh->exit_status` with the right child’s exit code.
 * - Handles signal-based exits (e.g., SIGINT).
 *
 * @param msh The shell context.
 * @param pid_left PID of the left child process.
 * @param pid_right PID of the right child process.
 */
void	wait_for_child(t_msh *msh, pid_t pid_left, pid_t pid_right)
{
	int	status_left;
	int	status_right;

	waitpid(pid_left, &status_left, 0);
	waitpid(pid_right, &status_right, 0);
	if (WIFEXITED(status_right))
		msh->exit_status = WEXITSTATUS(status_right);
	else if (WIFSIGNALED(status_right))
		msh->exit_status = 128 + WTERMSIG(status_right);
}
