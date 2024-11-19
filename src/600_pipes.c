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
	if (tokens->next == NULL)
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
	int		status;

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
		apply_pipe(tokens->left, msh, fd, 1);
		free_arg(msh->envp);
		free_msh(msh->cmds, msh, tokens->left);
		exit(EXIT_SUCCESS);
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
		exit(EXIT_SUCCESS);
	}
	ft_close(fd);
	waitpid(pid_left, &status, 0);
	waitpid(pid_right, &status, 0);
	return (SUCCESS);
}

















// static void	child_process(char **commands, int *fd, t_msh *msh, t_tkn *tokens);
// static void	parent_process(char **commands, int *fd, t_msh *msh, t_tkn *tokens);
// static void	pipe_execute(char *cmd, t_msh *msh, t_tkn *tokens, int *fd);
//
// int	exec_pipe_one(char **commands, t_msh *msh, t_tkn *tokens)
// {
// 	int		fd[2];
// 	int		pid;
// 	int		status;
//
// 	if (pipe(fd) == -1)
// 		perror("pipe");
// 	pid = fork();
// 	if (pid == -1)
// 	{
// 		perror("fork");
// 		ft_close(fd);
// 	}
// 	else if (pid == 0)
// 		child_process(commands, fd, msh, tokens);
// 	waitpid(pid, &status, 0);
// 	parent_process(commands, fd, msh, tokens);
// 	return (SUCCESS);
// }
//
// static void	child_process(char **commands, int *fd, t_msh *msh, t_tkn *tokens)
// {
// 	int	fd_op;
//
// 	fd_op = 0;
// 	close(fd[0]);
// 	if (fd_op == -1)
// 	{
// 		perror(commands[1]);
// 		ft_close(fd);
// 		exit (EXIT_FAILURE);
// 	}
// 	dup2(fd[1], STDOUT_FILENO);
// 	pipe_execute(commands[0], msh, tokens, fd);
// }
//
// static void	parent_process(char **commands, int *fd, t_msh *msh, t_tkn *tokens)
// {
// 	int	fd_op;
//
// 	fd_op = 0;
// 	close(fd[1]);
// 	if (fd_op == -1)
// 	{
// 		perror(commands[4]);
// 		ft_close(fd);
// 		exit (EXIT_FAILURE);
// 	}
// 	dup2(fd[0], STDIN_FILENO);
// 	pipe_execute(commands[0], msh, tokens, fd);
// }
//
// static void	pipe_execute(char *cmd, t_msh *msh, t_tkn *tokens, int *fd)
// {
// 	char	*path;
//
// 	path = find_path(cmd, msh->envp);
// 	if (!path)
// 	{
// 		printf("%s : command not found\n", cmd);
// 		free_msh(msh->cmds, msh, tokens);
// 		free_array(msh->envp, 0);
// 		exit(0);
// 	}
// 	if (execve(path, &cmd, msh->envp) == -1)
// 	{
// 		free(cmd);
// 		ft_close(fd);
// 		exit(127);
// 	}
// }
