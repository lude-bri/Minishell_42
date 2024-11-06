/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   600_pipes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:27:21 by luigi             #+#    #+#             */
/*   Updated: 2024/11/06 12:58:50 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


static void	child_process(char **commands, int *fd, t_msh *msh, t_tkn *tokens);
static void	parent_process(char **commands, int *fd, t_msh *msh, t_tkn *tokens);

int	exec_pipe_one(char **commands, t_msh *msh, t_tkn *tokens)
{
	int		fd[2];
	int		pid;

	if (pipe(fd) == -1)
		perror("pipe");
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		ft_close(fd);
	}
	else if (pid == 0)
		child_process(commands, fd, msh, tokens);
	parent_process(commands, fd, msh, tokens);
	return (SUCCESS);
}

static void	child_process(char **commands, int *fd, t_msh *msh, t_tkn *tokens)
{
	int	fd_op;

	fd_op = 0;
	close(fd[0]);
	if (fd_op == -1)
	{
		perror(commands[1]);
		ft_close(fd);
		exit (EXIT_FAILURE);
	}
	dup2(fd[1], STDOUT_FILENO);
	execute(commands, msh, tokens);
}

static void	parent_process(char **commands, int *fd, t_msh *msh, t_tkn *tokens)
{
	int	fd_op;

	fd_op = 0;
	close(fd[1]);
	if (fd_op == -1)
	{
		perror(commands[4]);
		ft_close(fd);
		exit (EXIT_FAILURE);
	}
	dup2(fd[0], STDIN_FILENO);
	execute(commands, msh, tokens);
}
