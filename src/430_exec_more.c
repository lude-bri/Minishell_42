/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   430_exec_more.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:57:51 by luigi             #+#    #+#             */
/*   Updated: 2024/11/06 12:37:07 by luigi            ###   ########.fr       */
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

static int	exec_pipe(char **command, t_msh *msh, t_tkn *tokens)
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
		apply_pipe(msh->tree_head->left, msh, fd, 1);
		exec_more(command, msh, tokens);
	}
	pid_right = fork();
	if (pid_right == -1)
		ft_close(fd);
	else if (pid_right == 0)
		apply_pipe(msh->tree_head->right, msh, fd, 0);
	close(fd[0]);
	close(fd[1]);
	free_msh(NULL, msh, tokens);
	waitpid(pid_left, &status, 0);
	waitpid(pid_right, &status, 0);
	return (SUCCESS);
}

int	exec_more(char **command, t_msh *msh, t_tkn *tokens)
{
	if (msh->tree_head->type == TKN_PIPE)
	{
		if (exec_pipe(command, msh, tokens) != SUCCESS)
			return (FAILURE);
	}
	else
	{
		if (exec_exe(tokens, command, msh) != SUCCESS)
			return (FAILURE);
	}
	return (SUCCESS);
}
