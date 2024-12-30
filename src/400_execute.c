/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   400_execute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:32:38 by luigi             #+#    #+#             */
/*   Updated: 2024/12/23 00:21:16 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	execute_builtin_commands(t_tkn *tokens, t_msh *msh)
{
	if (tokens->cmd_type == CMD_CD)
		msh->exit_status = msh_cd(tokens->cmdargs, msh->envp);
	else if (tokens->cmd_type == CMD_ENV)
		msh_env(msh->envp);
	else if (tokens->cmd_type == CMD_PWD)
		msh_pwd();
	else if (tokens->cmd_type == CMD_ECHO)
		msh_echo(msh->cmds->av, msh, tokens);
	else if (tokens->cmd_type == CMD_EXIT)
		msh_exit(tokens->cmdargs, msh, tokens);
	else if (tokens->cmd_type == CMD_UNSET)
		msh_unset(msh->cmds->av, &(msh->envp));
	else if (tokens->cmd_type == CMD_EXPORT)
	{
		if (msh->cmds->av[1]
			&& ((ft_strcmp(msh->cmds->av[1], ">") != 0)
				&& (ft_strcmp(msh->cmds->av[1], ">>") != 0)))
		{
			if (msh_export(msh, &(msh->ex_envp), msh->cmds->av[1]) == 1)
				msh->exit_status = 1;
		}
		else
			msh_export_no_var(msh->ex_envp);
	}
}

int	exec_bi(t_tkn *tokens, t_msh *msh)
{
	int	fd_in;
	int	fd_out;

	handle_redirections(tokens, msh, &fd_in, &fd_out);
	execute_builtin_commands(tokens, msh);
	dup2(fd_in, STDIN_FILENO);
	dup2(fd_out, STDOUT_FILENO);
	close(fd_in);
	close(fd_out);
	return (SUCCESS);
}

void	exec_special(t_tkn *tokens, t_msh *msh)
{
	char	*path;

	path = find_path(msh->cmds->av[0], msh->envp);
	if (!path)
	{
		write(STDERR_FILENO, " command not found\n", 19);
		free_msh(msh->cmds, msh, tokens);
		free_array(msh->envp, 0);
		exit(127);
	}
	if (msh->flag_redir == true)
	{
		if (execve(path, msh->cmds->av, msh->envp) == -1)
		{
			perror(path);
			free(path);
			free_arg(msh->cmds->av);
			free_msh(msh->cmds, msh, tokens);
			free_array(msh->envp, 0);
			exit(126);
		}
	}
}

void	execute(t_msh *msh, t_tkn *tokens)
{
	char	*path;
	char	**args;

	if (msh->flag_redir == true)
		exec_special(tokens, msh);
	path = find_path(tokens->name, msh->envp);
	args = build_args(tokens);
	if (!path)
	{
		write(STDERR_FILENO, " command not found\n", 19);
		free_msh(msh->cmds, msh, tokens);
		free_array(msh->envp, 0);
		free_arg(args);
		exit(127);
	}
	if (execve(path, args, msh->envp) == -1)
	{
		perror(path);
		free(path);
		free_arg(args);
		free_msh(msh->cmds, msh, tokens);
		free_array(msh->envp, 0);
		exit(126);
	}
}

int	exec_exe(t_tkn *tokens, t_msh *msh)
{
	int	status;

	if (ft_strcmp(tokens->name, "sudo") == 0)
		return (printf("msh: permission denied: sudo\n"), msh->exit_status = 126
			, SUCCESS);
	msh->pid = fork();
	if (msh->pid == 0)
	{
		exec_redirs(tokens, msh);
		execute(msh, tokens);
	}
	else if (msh->pid > 0)
	{
		waitpid(msh->pid, &status, 0);
		if (WIFEXITED(status))
			msh->exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			msh->exit_status = 128 + WTERMSIG(status);
	}
	else
	{
		perror("fork");
		msh->exit_status = 1;
	}
	return (SUCCESS);
}
