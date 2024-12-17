/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   600_pipes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:27:21 by luigi             #+#    #+#             */
/*   Updated: 2024/11/14 10:41:36 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	verify_pipe(t_tkn *tokens)
{
	if (!tokens->next)
		return (FAILURE);
	return (SUCCESS);
}

void	apply_pipe(t_tkn *dir, t_msh *msh, int *fd,
		int multi_fd)
{
	if (dup2(fd[multi_fd], multi_fd) == -1)
	{
		ft_close(fd);
		free_arg(msh->envp);
		free_arg(msh->cmds->av);
		exit(127);
	}
	ft_close(fd);
	exec_more(msh, dir);
}

int	exec_pipe(t_msh *msh, t_tkn *tokens)
{
	int		fd[2];
	pid_t	pid_left;
	pid_t	pid_right;
	int		heredoc_fd;
	int		status_left;
	int		status_right;

	if (verify_pipe(tokens) == FAILURE)
		return (SUCCESS);
	if (pipe(fd) < 0)
		return (FAILURE);
	pid_left = fork();
	if (pid_left == -1)
	{
		ft_close(fd);
		return (FAILURE);
	}
	else if (pid_left == 0)
	{
		close(fd[0]);
		if (tokens->left->type == TKN_HEREDOC || tokens->left->next->type == TKN_HEREDOC)
        {
			if (tokens->left->type == TKN_HEREDOC)
			{
				heredoc_fd = heredoc_pipe(tokens, msh, tokens->left->next->name, 1);
				dup2(heredoc_fd, STDIN_FILENO);
				apply_pipe(tokens->next->left, msh, fd, heredoc_fd);
				// free_arg(msh->envp);
				// free_msh(msh->cmds, msh, tokens->left);
				exit(msh->exit_status);
			}
			else
			{
				heredoc_fd = heredoc_pipe(tokens, msh, tokens->left->next->next->name, 0);
				dup2(heredoc_fd, STDIN_FILENO);
				apply_pipe(tokens->left, msh, fd, heredoc_fd);
				// free_arg(msh->envp);
				// free_msh(msh->cmds, msh, tokens->left);
				exit(msh->exit_status);
			}
        }
		apply_pipe(tokens->left, msh, fd, 1);
		free_arg(msh->envp);
		free_msh(msh->cmds, msh, tokens->left);
		exit(msh->exit_status);
	}
	pid_right = fork();
	if (pid_right == -1)
	{
		ft_close(fd);
		return (FAILURE);
	}
	else if (pid_right == 0)
	{
		close(fd[1]);
		apply_pipe(tokens->right, msh, fd, 0);
		free_arg(msh->envp);
		free_msh(msh->cmds, msh, tokens->left);
		exit(msh->exit_status);
	}
	ft_close(fd);
	waitpid(pid_left, &status_left, 0);
	waitpid(pid_right, &status_right, 0);
	// free_msh(msh->cmds, msh, tokens);
    if (WIFEXITED(status_right)) // Check if right child exited normally
        msh->exit_status = WEXITSTATUS(status_right);
    else if (WIFSIGNALED(status_right)) // Check if right child terminated by a signal
        msh->exit_status = 128 + WTERMSIG(status_right);
    return (SUCCESS);
}
