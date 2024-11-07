/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   910_close.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:33:26 by luigi             #+#    #+#             */
/*   Updated: 2024/11/06 12:33:48 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_close(int *fd)
{
	close(fd[0]);
	close(fd[1]);
}