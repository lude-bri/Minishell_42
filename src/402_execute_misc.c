/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   402_execute_misc.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 23:47:58 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/08 17:21:37 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Orchestrates the execution of parsed commands.
 *
 * - Runs heredoc processing if needed.
 * - Builds the binary tree from the token list.
 * - Executes either a single command (`exec_one`) or multiple piped commands (`exec_more`).
 *
 * @param msh Pointer to the shell state.
 * @param tokens Pointer to the token list.
 * @return SUCCESS on success; FAILURE on execution error.
 */
int	to_execute(t_msh *msh, t_tkn *tokens)
{
	if (find_heredoc(tokens))
		heredoc_exec(msh, tokens);
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

/**
 * @brief Prepares and applies input/output redirections for a command.
 *
 * Saves original `stdin` and `stdout` using `dup()`, then calls `exec_redirs()`
 * to apply redirection logic (e.g., `<`, `>`, `>>`, `<<`).
 *
 * @param tokens Pointer to the command token.
 * @param msh Pointer to the shell state.
 * @param fd_in Pointer to an int for storing the original stdin fd.
 * @param fd_out Pointer to an int for storing the original stdout fd.
 */
void	handle_redirections(t_tkn *tokens, t_msh *msh, int *fd_in, int *fd_out)
{
	(void)fd_in;
	(void)fd_out;
	msh->fd_in = 0;
	msh->fd_out = 0;
	msh->fd_in = dup(STDIN_FILENO);
	msh->fd_out = dup(STDOUT_FILENO);
	exec_redirs(tokens, msh);
}

/**
 * @brief Checks if a command is a direct path to an executable.
 *
 * Accepts absolute paths (`/...`) or relative paths (`./...`).
 * Also detects heredoc markers (`<<`) for special handling.
 *
 * @param cmd The command string.
 * @return A duplicated path string if valid; "FOUND_IN" if heredoc; NULL otherwise.
 */
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
		if (cmd[0] == '<' && cmd[1] == '<')
			return ("FOUND_IN");
	}
	return (NULL);
}

/**
 * @brief Searches for the executable in the system's PATH environment variable.
 *
 * Constructs full paths from each PATH entry and checks accessibility with `access()`.
 *
 * @param cmd The name of the command (e.g., `ls`).
 * @param envp The environment variables array.
 * @return A heap-allocated string with the full path if found; NULL otherwise.
 */
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

/**
 * @brief Resolves the full path to a command.
 *
 * - First checks if it's a direct path.
 * - If not, scans `$PATH` entries for a matching executable.
 * - Skips non-command tokens during resolution.
 *
 * @param cmd The command name as passed by the user.
 * @param envp The environment array.
 * @param tokens Token list to ensure we resolve the correct command.
 * @return Full executable path or NULL if not found.
 */
char	*find_path(char *cmd, char **envp, t_tkn *tokens)
{
	char	*path;

	path = check_direct_path(cmd);
	if (path)
		return (path);
	while (tokens->type != TKN_CMD)
		tokens = tokens->next;
	return (search_in_env_path(tokens->name, envp));
}
