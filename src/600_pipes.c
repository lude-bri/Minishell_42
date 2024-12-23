/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   600_pipes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:27:21 by luigi             #+#    #+#             */
/*   Updated: 2024/12/23 02:12:14 by mde-agui         ###   ########.fr       */
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

int	setup_pipe_and_fork(t_msh *msh, t_tkn *tokens, int *fd)
{
	pid_t	pid_left;
	pid_t	pid_right;

	if (verify_pipe(tokens) == FAILURE)
		return (SUCCESS);
	if (pipe(fd) < 0)
		return (FAILURE);
	pid_left = fork();
	if (pid_left == -1)
		return (ft_close(fd), FAILURE);
	else if (pid_left == 0)
		handle_left_child(tokens, msh, fd);
	pid_right = fork();
	if (pid_right == -1)
		return (ft_close(fd), FAILURE);
	else if (pid_right == 0)
		handle_right_child(tokens, msh, fd);
	ft_close(fd);
	wait_for_child(msh, pid_left, pid_right);
	return (SUCCESS);
}

int	exec_pipe(t_msh *msh, t_tkn *tokens)
{
	int	fd[2];

	return (setup_pipe_and_fork(msh, tokens, fd));
}
