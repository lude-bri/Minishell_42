/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   400_execute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:32:38 by luigi             #+#    #+#             */
/*   Updated: 2024/10/31 16:09:07 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	exec_bi(t_tkn *tokens, char **command, t_msh *msh);
static int	exec_exe(t_tkn *tokens, char **command, t_msh *msh);
static void	execute(char **cmd, char **envp);
static char	*find_path(char *cmd, char **envp);

int	to_execute(char **command, t_msh *msh, t_tkn *tokens)
{
	if (tokens->type == TKN_CMD)
		if (exec_bi(tokens, command, msh))
			if (!exec_exe(tokens, command, msh))
				return (FAILURE);
	return (SUCCESS);
}

static int	exec_bi(t_tkn *tokens, char **command, t_msh *msh)
{
	if (tokens->cmd_type == CMD_CD)
		msh_cd(command);
	if (tokens->cmd_type == CMD_ENV)
		msh_env(msh->envp);
	if (tokens->cmd_type == CMD_PWD)
		msh_pwd();
	if (tokens->cmd_type == CMD_ECHO)
		msh_echo(command);
	if (tokens->cmd_type == CMD_EXIT)
		msh_exit(command, msh);
	if (tokens->cmd_type == CMD_UNSET)
		msh_unset(command, &(msh->envp));
	if (tokens->cmd_type == CMD_EXPORT)
		msh_export(msh->envp);
	else
		return (FAILURE);
	return (SUCCESS);
}

static int	exec_exe(t_tkn *tokens, char **command, t_msh *msh)
{
	int		pid;
	int		status;

	(void)tokens;
	pid = fork();
	if (pid == 0)
		execute(command, msh->envp);
	waitpid(pid, &status, 0);
	return (SUCCESS);
}

static void	execute(char **cmd, char **envp)
{
	char	*path;

	path = find_path(cmd[0], envp);
	if (!path)
	{
		free_arg(cmd);
		ft_putstr_fd("command not found\n", 2);
		exit(0);
	}
	if (execve(path, cmd, envp) == -1)
	{
		free_arg(cmd);
		exit(127);
	}
}


static char	*find_path(char *cmd, char **envp)
{
	char	**full_path;
	char	*half_path;
	char	*path;
	int		i;

	i = 0;
	if (!envp[i])
		return (NULL);
	while (ft_strnstr(envp[i], "PATH=", 5) == 0)
		i++;
	full_path = ft_split(envp[i] + 5, ':');
	i = 0;
	while (full_path[i])
	{
		half_path = ft_strjoin(full_path[i], "/");
		path = ft_strjoin(half_path, cmd);
		free(half_path);
		if (access(path, F_OK | X_OK) == 0)
			return (free_arg(full_path), path);
		free(path);
		i++;
	}
	return (free_arg(full_path), NULL);
}

/////////////////////////////////
////         OLD EXEC        ////
/////////////////////////////////

	// i = -1;
	// while (command[++i])
	// 	printf("Arg number [%d] -> %s\n", i, command[i]);
	// if (command[0] && ft_strcmp(command[0], "exit") == 0)
	// 		msh_exit(command, msh);
	// if (command[0] && ft_strcmp(command[0], "pwd") == 0)
	// 	msh_pwd();
	// if (command[0] && ft_strcmp(command[0], "echo") == 0)
	// 	msh_echo(command);
	// if (command[0] && ft_strcmp(command[0], "cd") == 0)
	// 	msh_cd(command);
	// if (command[0] && ft_strcmp(command[0], "export") == 0)
	// 	msh_export(msh->envp);
	// if (command[0] && ft_strcmp(command[0], "env") == 0)
	// 	msh_env(msh->envp);
	// if (command[0] && ft_strcmp(command[0], "unset") == 0)
	// 	msh_unset(command, &(msh->envp));
	// return (SUCCESS);
