/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   730_redir_app.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 08:03:26 by luigi             #+#    #+#             */
/*   Updated: 2024/12/23 02:52:18 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	redir_append(t_tkn *tokens, t_msh *msh)
{
	int	fd;

	tokens = tokens->next->next;
	fd = open(tokens->name, O_WRONLY | O_CREAT | O_APPEND, 0775);
	if (fd < 0)
	{
		perror(tokens->name);
		free_msh(msh->cmds, msh, tokens);
		free_arg(msh->arg);
		free_arg(msh->envp);
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
