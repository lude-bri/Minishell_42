/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   710_redir_in.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 07:50:15 by luigi             #+#    #+#             */
/*   Updated: 2024/11/18 08:09:08 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Handles input redirection (`<`) for a command.
 *
 * Opens the file specified by the next token in read-only mode and redirects
 * the file descriptor to `STDIN_FILENO` using `dup2()`. If opening the file
 * fails, it prints an error, frees all allocated resources, and exits the program.
 *
 * @param tokens The current token node (should be of type TKN_IN).
 *               The next token must contain the filename.
 * @param msh The main minishell context structure holding environment,
 *            arguments, and command data.
 */
void	redir_in(t_tkn *tokens, t_msh *msh)
{
	int		fd;

	tokens = tokens->next;
	fd = open(tokens->name, O_RDONLY);
	if (fd < 0)
	{
		perror(tokens->name);
		free_msh(msh->cmds, msh, tokens);
		free_arg(msh->envp);
		free_arg(msh->ex_envp);
		free_arg(msh->arg);
		exit(EXIT_FAILURE);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
}
