/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   120_prompt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 12:23:10 by luigi             #+#    #+#             */
/*   Updated: 2024/10/19 12:44:33 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*build_prompt(t_msh *msh)
{
	char	*line;

	line = readline("\001\033[1;32m\002msh$ \001\033[0m\002");
	if (!line)
	{
		printf("exit\n");
		free_arg(msh->envp);
		exit (0);
	}
	else if (line[0] == '\0')
		return (NULL);
	return (line);
}
