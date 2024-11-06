/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   430_exec_more.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:57:51 by luigi             #+#    #+#             */
/*   Updated: 2024/11/06 12:25:12 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	exec_more(char **command, t_msh *msh, t_tkn *tokens)
{
	int		pipe1[2];
	int		pipe2[2];
	int		cmd;

	if (exec_pipe_one(command, msh, pipe1) != SUCCESS)
		return (FAILURE);
	(void)command;
	(void)msh;
	(void)tokens;
	return (SUCCESS);
}

int	main(int ac, char **av, char **envp)
{
	int		i;
	// int		fd_in;
	// int		fd_out;

	if (ac >= 5)
	{
		i = 2;
		// fd_in = fd_open(av[1], 2);
		// fd_out = fd_open(av[ac -1], 1);
		dup2(fd_in, STDIN_FILENO);
		while (i < ac - 2)
			main_process(&av[i++], envp);
		dup2(fd_out, STDOUT_FILENO);
		execute(av[ac - 2], envp, NULL);
	}
	ft_putstr_fd("Error! Try it ./pipex file1 cmd1 cmd2 cmdn cmd3 file2", 2);
}

void	main_process(char **av, char **envp)
{
	int		fd[2];
	pid_t	pid;

	if (pipe(fd) == -1)
		perror("pipe");
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		ft_close(fd);
	}
	if (pid == 0)
	{
		close(fd[0]);
		dup2(fd[1], STDOUT_FILENO);
		execute(*av, envp, fd);
	}
	close(fd[1]);
	dup2(fd[0], STDIN_FILENO);
	waitpid(pid, NULL, 0);
}
