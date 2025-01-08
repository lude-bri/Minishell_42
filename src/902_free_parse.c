/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   902_free_parse.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 17:05:34 by luigi             #+#    #+#             */
/*   Updated: 2025/01/08 19:32:40 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_parse(t_msh *msh)
{
	free_arg(msh->cmds->av);
	free(msh->cmds);
	// if (msh->heredoc)
	// 	free(msh->heredoc);
}
