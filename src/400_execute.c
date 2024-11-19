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

// static int	exec_redirs_bi(t_tkn *tokens, t_msh *msh);

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
		msh_cd(tokens->cmdargs);
	else if (tokens->cmd_type == CMD_ENV)
		msh_env(msh->envp);
	else if (tokens->cmd_type == CMD_PWD)
		msh_pwd();
	else if (tokens->cmd_type == CMD_ECHO)
		msh_echo(tokens->cmdargs);
		// msh_echo(msh, tokens);
	else if (tokens->cmd_type == CMD_EXIT)
		msh_exit(tokens->cmdargs, msh);
	else if (tokens->cmd_type == CMD_UNSET)
		msh_unset(tokens->cmdargs, &(msh->envp));
	else if (tokens->cmd_type == CMD_EXPORT)
	{
		if (msh->cmds->av[1] && (ft_strcmp(msh->cmds->av[1], ">") != 0))
			msh_export(&(msh->envp), msh->cmds->av[1]);
		else
			msh_export_no_var(msh->envp);
	}
	// else
	// 	return (FAILURE);
	dup2(fd_in, STDIN_FILENO);
	dup2(fd_out, STDOUT_FILENO);
	close(fd_in);
	close(fd_out);
	return (SUCCESS);
}

// static int	exec_redirs_bi(t_tkn *tokens, t_msh *msh)
// {
// 	int		fd_in;
// 	int		fd_out;
//
// 	fd_in = dup(STDIN_FILENO);
// 	fd_out = dup(STDOUT_FILENO);
// 	while (tokens)
// 	{
// 		if (tokens->type == TKN_HEREDOC)
// 		{
// 			heredoc(tokens, msh);
// 			return (SUCCESS);
// 		}
// 		if (tokens->type == TKN_IN || tokens->type == TKN_OUT
// 			|| tokens->type == TKN_APPEND)
// 		{
// 			redirs(tokens, msh);
// 		}
// 		tokens = tokens->next;
// 	}
// 	dup2(fd_in, STDIN_FILENO);
// 	dup2(fd_out, STDOUT_FILENO);
// 	close(fd_in);
// 	close(fd_out);
// 	return (SUCCESS);
// }

//testing redirections
int exec_exe(t_tkn *tokens, t_msh *msh)
{
    int pid;
    int status;

    pid = fork();
    if (pid == 0)
    {
        exec_redirs(tokens, msh);
        execute(msh, tokens);
    }
    waitpid(pid, &status, 0);
    return (SUCCESS);
}

void	execute(t_msh *msh, t_tkn *tokens)
{
	char	*path;
	char	**args;

	path = find_path(tokens->name, msh->envp);
	args = build_args(tokens);
	if (!path)
	{
		printf("%s: command not found\n", tokens->cmdargs[0]);
		free_msh(msh->cmds, msh, tokens);
		free_array(msh->envp, 0);
		free_arg(args);
		exit(0);
	}
	if (execve(path, args, msh->envp) == -1)
	{
		perror(path);
		free(path);
		free_arg(args);
		free_msh(msh->cmds, msh, tokens);
		free_array(msh->envp, 0);
		exit(127);
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
