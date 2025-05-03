/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   400_execute.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:32:38 by luigi             #+#    #+#             */
/*   Updated: 2025/01/12 11:44:30 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Handles the `export` built-in command.
 *
 * If arguments are provided and not invalid tokens (`>`, `>>`, `|`), it exports them to `ex_envp`.
 * If no variable is provided, it lists the current exported variables.
 *
 * @param tokens Pointer to the token representing the command.
 * @param msh Pointer to the shell state.
 */
void	execute_export_command(t_tkn *tokens, t_msh *msh)
{
	int	i;

	(void)tokens;
	i = 0;
	if (msh->cmds->av[1]
		&& ((ft_strcmp(msh->cmds->av[1], ">") != 0)
			&& (ft_strcmp(msh->cmds->av[1], ">>") != 0)
			&& (ft_strcmp(msh->cmds->av[1], "|") != 0)))
	{
		while (msh->cmds->av[++i])
		{
			if (msh_export(msh, &(msh->ex_envp), msh->cmds->av[i]) == 1)
				msh->exit_status = 1;
		}
	}
	else
		msh_export_no_var(msh->ex_envp);
}

/**
 * @brief Executes built-in commands such as `cd`, `echo`, `exit`, etc.
 *
 * Determines the built-in type from `cmd_type` and calls the appropriate handler function.
 *
 * @param tokens Pointer to the current command token.
 * @param msh Pointer to the shell state.
 */
void	execute_builtin_commands(t_tkn *tokens, t_msh *msh)
{
	if (tokens->cmd_type == CMD_CD)
		msh->exit_status = msh_cd(tokens->cmdargs, msh->envp);
	else if (tokens->cmd_type == CMD_ENV)
		msh_env(msh->envp, msh, tokens);
	else if (tokens->cmd_type == CMD_PWD)
		msh_pwd(msh, tokens);
	else if (tokens->cmd_type == CMD_ECHO)
		msh_echo(msh->cmds->av, msh, tokens);
	else if (tokens->cmd_type == CMD_EXIT)
		msh_exit(tokens->cmdargs, msh, tokens);
	else if (tokens->cmd_type == CMD_UNSET)
	{
		msh_unset(msh->cmds->av, &(msh->envp));
		msh_unset(msh->cmds->av, &(msh->ex_envp));
	}
	else if (tokens->cmd_type == CMD_EXPORT)
		execute_export_command(tokens, msh);
}

/**
 * @brief Executes a built-in command with proper redirection handling.
 *
 * - Applies input/output redirections.
 * - Calls the built-in command.
 * - Restores original file descriptors using `dup2`.
 *
 * @param tokens Pointer to the token representing the command.
 * @param msh Pointer to the shell state.
 * @return Always returns SUCCESS.
 */
int	exec_bi(t_tkn *tokens, t_msh *msh)
{
	int	fd_in;
	int	fd_out;

	fd_in = 0;
	fd_out = 0;
	handle_redirections(tokens, msh, &fd_in, &fd_out);
	execute_builtin_commands(tokens, msh);
	dup2(fd_in, STDIN_FILENO);
	dup2(fd_out, STDOUT_FILENO);
	return (SUCCESS);
}

/**
 * @brief Executes an external binary using `execve`.
 *
 * - Resolves the binary path.
 * - Builds the argument array.
 * - Handles execution errors (e.g., command not found, permission denied).
 * - Frees memory and exits in child if necessary.
 *
 * @param msh Pointer to the shell state.
 * @param tokens Pointer to the command token.
 */
void	execute(t_msh *msh, t_tkn *tokens)
{
	char	*path;
	char	**args;

	path = find_path(tokens->name, msh->envp, tokens);
	args = build_args(tokens);
	if (!path)
		handle_command_not_found(msh, tokens, args);
	if (ft_strcmp(path, "FOUND_IN") == 0)
	{
		free_msh(msh->cmds, msh, tokens);
		free_array(msh->envp, 0);
		free_arg(msh->ex_envp);
		free_arg(args);
		exit(127);
	}
	if (execve(path, args, msh->envp) == -1)
		handle_execve_error(path, args, msh, tokens);
}

/**
 * @brief Executes an external command by forking and calling `execve` in the child.
 *
 * - Handles special case for `sudo`, blocking its use with an error.
 * - Configures signal handling to default.
 * - Forks the process.
 *   - Child: handles command execution via `handle_child_process()`.
 *   - Parent: waits and sets the exit status via `handle_parent_process()`.
 *
 * @param tokens Pointer to the command token.
 * @param msh Pointer to the shell state.
 * @return Always returns SUCCESS.
 */
int	exec_exe(t_tkn *tokens, t_msh *msh)
{
	int					status;
	struct sigaction	sa_default;

	status = 0;
	if (ft_strcmp(tokens->name, "sudo") == 0)
		return (printf("msh: permission denied: sudo\n")
			, msh->exit_status = 126, SUCCESS);
	sa_default.sa_handler = SIG_DFL;
	sigemptyset(&sa_default.sa_mask);
	sa_default.sa_flags = 0;
	sigaction(SIGINT, &sa_default, NULL);
	sigaction(SIGQUIT, &sa_default, NULL);
	msh->pid = fork();
	if (msh->pid == 0)
		handle_child_process(tokens, msh);
	else if (msh->pid > 0)
		handle_parent_process(msh, status);
	else
	{
		perror("fork");
		msh->exit_status = 1;
	}
	return (SUCCESS);
}
