/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   430_exec_more.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:57:51 by luigi             #+#    #+#             */
/*   Updated: 2024/11/09 14:09:29 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	apply_pipe(t_tkn *dir, t_msh *msh, int *fd,
		int multipurp_fd)
{
	(void)dir;
	(void)msh;
	if (dup2(fd[multipurp_fd], multipurp_fd) == -1)
	{
		close(fd[0]);
		close(fd[1]);
	}
	close(fd[0]);
	close(fd[1]);
}

static int	exec_pipe(t_msh *msh, t_tkn *tokens)
{
	int		fd[2];
	pid_t	pid_left;
	pid_t	pid_right;
	int		status;

	if (pipe(fd) < 0)
		return (FAILURE);
	pid_left = fork();
	if (pid_left == -1)
		ft_close(fd);
	else if (pid_left == 0)
	{
		apply_pipe(tokens->left, msh, fd, 1);
		exec_more(msh, tokens->left);
	}
	pid_right = fork();
	if (pid_right == -1)
		ft_close(fd);
	else if (pid_right == 0)
	{
		apply_pipe(tokens->right, msh, fd, 0);
		exec_more(msh, tokens->right);
	}
	close(fd[0]);
	close(fd[1]);
	free_msh(NULL, msh, tokens);
	waitpid(pid_left, &status, 0);
	waitpid(pid_right, &status, 0);
	return (SUCCESS);
}

int	exec_more(t_msh *msh, t_tkn *tokens)
{
	if (tokens->type == TKN_PIPE)
	{
		// printf("exec_pipe activated\n");
		if (exec_pipe(msh, tokens) != SUCCESS)
		 	return (FAILURE);
	}
	else
	{
		if (exec_exe(tokens, msh) != SUCCESS)
			return (FAILURE);
	}
	return (SUCCESS);
}
