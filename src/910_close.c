/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   910_close.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:33:26 by luigi             #+#    #+#             */
/*   Updated: 2025/01/08 19:29:00 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_and_close_all(t_msh *msh, t_tkn *tokens)
{
	free_arg(msh->envp);
	free_arg(msh->ex_envp);
	free_msh(msh->cmds, msh, tokens);
	close(msh->fd_in);
	close(msh->fd_out);
}

void	ft_close(int *fd)
{
	close(fd[0]);
	close(fd[1]);
}
