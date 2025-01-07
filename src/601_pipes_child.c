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

// void	handle_heredoc(t_tkn *tokens, t_msh *msh, int *fd)
// {
// 	int	heredoc_fd;
//
// 	if (tokens->left->type == TKN_HEREDOC)
// 	{
// 		heredoc_fd = heredoc_pipe(tokens, msh, tokens->left->next->name, 1);
// 		dup2(heredoc_fd, STDIN_FILENO);
// 		if (tokens->next->left != NULL)
// 			apply_pipe(tokens->next->left, msh, fd, heredoc_fd);
// 		free_msh(msh->cmds, msh, tokens->left);
// 		exit(msh->exit_status);
// 	}
// 	else
// 	{
// 		heredoc_fd = heredoc_pipe(tokens, msh,
// 				tokens->left->next->next->name, 0);
// 		dup2(heredoc_fd, STDIN_FILENO);
// 		apply_pipe(tokens->left, msh, fd, heredoc_fd);
// 		exit(msh->exit_status);
// 	}
// }

void	handle_left_child(t_tkn *tokens, t_msh *msh, int *fd)
{
	close(fd[0]);
	apply_pipe(tokens->left, msh, fd, 1);
	free_arg(msh->envp);
	free_arg(msh->ex_envp);
	free_msh(msh->cmds, msh, tokens->left);
	exit(msh->exit_status);
}

void	handle_right_child(t_tkn *tokens, t_msh *msh, int *fd)
{
	close(fd[1]);
	apply_pipe(tokens->right, msh, fd, 0);
	free_arg(msh->envp);
	free_arg(msh->ex_envp);
	free_msh(msh->cmds, msh, tokens->left);
	exit(msh->exit_status);
}

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
