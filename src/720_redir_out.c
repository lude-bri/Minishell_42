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

void	redir_out(t_tkn *tokens, t_msh *msh)
{
	int		fd;

	tokens = tokens->next;
	fd = open(tokens->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
	{
		perror(tokens->name);
		free_msh(msh->cmds, msh, tokens);
		free_arg(msh->arg);
		free_arg(msh->envp);
		exit(EXIT_FAILURE);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
}
