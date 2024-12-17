/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   400_execute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:32:38 by luigi             #+#    #+#             */
/*   Updated: 2024/11/18 10:50:40 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	to_execute(t_msh *msh, t_tkn *tokens)
{
	msh->tree_head = create_bin_tree(tokens);
	tokens->cmdargs = msh->cmds->av;
	if (msh->tree_head->type != TKN_PIPE)
	{
		if (exec_one(msh, tokens) != SUCCESS)
			return (FAILURE);
	}
	else
	{
		if (exec_more(msh, msh->tree_head) != SUCCESS)
			return (FAILURE);
	}
	return (SUCCESS);
}

int	exec_bi(t_tkn *tokens, t_msh *msh)
{
	int		fd_in;
	int		fd_out;

	fd_in = dup(STDIN_FILENO);
	fd_out = dup(STDOUT_FILENO);
	exec_redirs(tokens, msh);
	if (tokens->cmd_type == CMD_CD)
		msh->exit_status = msh_cd(tokens->cmdargs, msh->envp);
	else if (tokens->cmd_type == CMD_ENV)
		msh_env(msh->envp);
	else if (tokens->cmd_type == CMD_PWD)
		msh_pwd();
	else if (tokens->cmd_type == CMD_ECHO)
		msh_echo(tokens->cmdargs, msh);
	else if (tokens->cmd_type == CMD_EXIT)
		msh_exit(tokens->cmdargs, msh);
	else if (tokens->cmd_type == CMD_UNSET)
		msh_unset(tokens->cmdargs, &(msh->envp));
	else if (tokens->cmd_type == CMD_EXPORT)
	{
		if (msh->cmds->av[1] 
			&& ((ft_strcmp(msh->cmds->av[1], ">") != 0)
			&& (ft_strcmp(msh->cmds->av[1], ">>") != 0)))
		{		
			if (msh_export(&(msh->envp), msh->cmds->av[1]) == 1)
				msh->exit_status = 1;
		}
		else
			msh_export_no_var(msh->envp);
	}
	dup2(fd_in, STDIN_FILENO);
	dup2(fd_out, STDOUT_FILENO);
	close(fd_in);
	close(fd_out);
	return (SUCCESS);
}

//testing redirections
int exec_exe(t_tkn *tokens, t_msh *msh)
{
    int pid;
    int status;

	//verify if is sudo
	if (ft_strcmp(tokens->name, "sudo") == 0)
	{
		printf("msh: permission denied: sudo\n");
		msh->exit_status = 126;
		return (SUCCESS);
	}
    pid = fork();
    if (pid == 0)
    {
        exec_redirs(tokens, msh);
        execute(msh, tokens);
    }
    else if (pid > 0) // Parent process
    {
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) // Check if child exited normally
            msh->exit_status = WEXITSTATUS(status); // Update exit status
        else if (WIFSIGNALED(status)) // Handle signals (if needed)
            msh->exit_status = 128 + WTERMSIG(status); // Signal-based exit
    }
    else // Fork failed
    {
        perror("fork");
        msh->exit_status = 1; // Indicate failure
    }
    // waitpid(pid, &status, 0);
    return (SUCCESS);
}

static void	exec_special(t_tkn *tokens, t_msh *msh)
{
	char	*path;

	path = find_path(msh->cmds->av[0], msh->envp);
	if (!path)
	{
		printf("%s: command not found\n", tokens->cmdargs[0]);
		free_msh(msh->cmds, msh, tokens);
		free_array(msh->envp, 0);
		free_arg(msh->cmds->av);
		exit(127);
	}
	if (msh->flag_redir == true)
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
		printf("%s: command not found\n", args[0]); 
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

char *find_path(char *cmd, char **envp)
{
    char **full_path;
    char *half_path;
    char *path;
    int i;

    if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
    {
        if (access(cmd, F_OK | X_OK) == 0)
            return (ft_strdup(cmd));
        return (NULL);
    }
    i = 0;
    while (envp[i] && ft_strnstr(envp[i], "PATH=", 5) == 0)
        i++;
    if (!envp[i])
        return (NULL);

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
