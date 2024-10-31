/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   550_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 10:32:18 by luigi             #+#    #+#             */
/*   Updated: 2024/10/31 17:10:43 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	msh_env(char **envp)
{
	int		i;
	int		j;

	if (!envp)
		return (1);
	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j])
		{
			if (write(1, &envp[i][j], 1) == -1)
				return (1);
			j++;
		}
		if (write (1, "\n", 1) == -1)
			return (1);
		i++;
	}
	return (0);
}
