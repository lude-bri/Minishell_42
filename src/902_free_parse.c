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

/**
 * @brief Frees the parsing-related structures in the shell.
 *
 * Specifically frees the command arguments array and the command structure.
 *
 * @param msh Pointer to the main shell state structure (t_msh).
 */
void	free_parse(t_msh *msh)
{
	free_arg(msh->cmds->av);
	free(msh->cmds);
}
