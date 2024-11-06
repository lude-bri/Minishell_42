/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   420_exec_one.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:40:02 by luigi             #+#    #+#             */
/*   Updated: 2024/11/06 11:41:54 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	exec_one(char **command, t_msh *msh, t_tkn *tokens)
{
	if (tokens->type == TKN_CMD)
		if (exec_bi(tokens, command, msh) == FAILURE)
			if (exec_exe(tokens, command, msh) != SUCCESS)
				return (FAILURE);
	return (SUCCESS);
}
