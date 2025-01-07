/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   403_execute_misc2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 17:09:35 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/07 17:10:18 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_command_not_found(t_msh *msh, t_tkn *tokens, char **args)
{
	write(STDERR_FILENO, " command not found\n", 19);
	free_msh(msh->cmds, msh, tokens);
	free_array(msh->envp, 0);
	free_arg(msh->ex_envp);
	free_arg(args);
	exit(127);
}

void	handle_execve_error(char *path, char **args, t_msh *msh, t_tkn *tokens)
{
	perror(path);
	free(path);
	free_arg(args);
	free_msh(msh->cmds, msh, tokens);
	free_array(msh->envp, 0);
	free_arg(msh->ex_envp);
	exit(126);
}
