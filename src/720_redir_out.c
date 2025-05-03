/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   720_redir_out.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 07:59:38 by luigi             #+#    #+#             */
/*   Updated: 2024/11/18 08:09:24 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Handles output redirection (`>`) for a command.
 *
 * Opens (or creates) the file specified by the next token in write-only mode,
 * truncating it if it exists. Then redirects the file descriptor to `STDOUT_FILENO`
 * using `dup2()`. On failure to open the file, it prints an error message,
 * frees all allocated memory and exits the program.
 *
 * @param tokens The current token node (should be of type TKN_OUT).
 *               The next token must contain the output filename.
 * @param msh The main minishell context structure holding environment,
 *            arguments, and command data.
 */
void	redir_out(t_tkn *tokens, t_msh *msh)
{
	int		fd;

	tokens = tokens->next;
	fd = -1;
	if (tokens != NULL)
		fd = open(tokens->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(tokens->name);
		free_msh(msh->cmds, msh, tokens);
		free_arg(msh->arg);
		free_arg(msh->envp);
		free_arg(msh->ex_envp);
		exit(EXIT_FAILURE);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}
