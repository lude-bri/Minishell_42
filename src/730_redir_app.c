/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   730_redir_app.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 08:03:26 by luigi             #+#    #+#             */
/*   Updated: 2024/11/18 09:11:40 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	redir_append(t_tkn *tokens, t_msh *msh)
{
	int		fd;

	tokens = tokens->next;
	fd = open(tokens->name, O_WRONLY | O_CREAT | O_APPEND, 0664);
	if (fd < 0)
	{
		perror(tokens->name);
		free_msh(msh->cmds, msh, tokens);
		exit(EXIT_FAILURE);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}
