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

/**
 * @brief Frees all major shell resources and closes file descriptors.
 *
 * Used typically when exiting a child process or handling errors during execution.
 *
 * @param msh Pointer to the main shell state structure (t_msh).
 * @param tokens Pointer to the current token list (t_tkn).
 */
void	free_and_close_all(t_msh *msh, t_tkn *tokens)
{
	free_arg(msh->envp);
	free_arg(msh->ex_envp);
	free_msh(msh->cmds, msh, tokens);
	close(msh->fd_in);
	close(msh->fd_out);
}

/**
 * @brief Closes both ends of a pipe file descriptor array.
 *
 * @param fd Array of two file descriptors (fd[0] for reading, fd[1] for writing).
 */
void	ft_close(int *fd)
{
	close(fd[0]);
	close(fd[1]);
}
