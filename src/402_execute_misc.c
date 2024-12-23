/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   402_execute_misc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 23:47:58 by mde-agui          #+#    #+#             */
/*   Updated: 2024/12/23 00:01:19 by mde-agui         ###   ########.fr       */
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

void	handle_redirections(t_tkn *tokens, t_msh *msh, int *fd_in, int *fd_out)
{
	*fd_in = dup(STDIN_FILENO);
	*fd_out = dup(STDOUT_FILENO);
	exec_redirs(tokens, msh);
}

char	*check_direct_path(char *cmd)
{
	if (cmd[0])
	{
		if (cmd[0] == '/' || (cmd[0] == '.' && cmd[1] == '/'))
		{
			if (access(cmd, F_OK | X_OK) == 0)
				return (ft_strdup(cmd));
			return (NULL);
		}
	}
	return (NULL);
}

char	*search_in_env_path(char *cmd, char **envp)
{
	char	**full_path;
	char	*half_path;
	char	*path;
	int		i;

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

char	*find_path(char *cmd, char **envp)
{
	char	*path;

	path = check_direct_path(cmd);
	if (path)
		return (path);
	return (search_in_env_path(cmd, envp));
}
