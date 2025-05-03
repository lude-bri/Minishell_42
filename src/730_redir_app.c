/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   730_redir_app.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 08:03:26 by luigi             #+#    #+#             */
/*   Updated: 2024/12/24 12:46:46 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Handles output redirection in append mode (`>>`) for a command.
 *
 * Opens (or creates) the file specified by the next token in write-only append mode.
 * If the file doesn't exist, it is created with permissions 0775. The file descriptor
 * is then duplicated to `STDOUT_FILENO` using `dup2()`, so the command output is appended
 * to the file. If `open()` or `dup2()` fails, an error is printed, all resources are freed,
 * and the shell exits with failure.
 *
 * @param tokens Pointer to the current token (should be of type TKN_APPEND).
 *               The next token must contain the filename for redirection.
 * @param msh Pointer to the main shell structure holding the current environment,
 *            command list, and arguments.
 */
void	redir_append(t_tkn *tokens, t_msh *msh)
{
	int	fd;

	tokens = tokens->next;
	fd = open(tokens->name, O_WRONLY | O_CREAT | O_APPEND, 0775);
	if (fd < 0)
	{
		perror(tokens->name);
		free_msh(msh->cmds, msh, tokens);
		free_arg(msh->arg);
		free_arg(msh->envp);
		free_arg(msh->ex_envp);
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDOUT_FILENO) < 0)
	{
		perror("dup2");
		free_msh(msh->cmds, msh, tokens);
		exit(EXIT_FAILURE);
	}
	close(fd);
}
