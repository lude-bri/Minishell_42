/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:49:02 by luigi             #+#    #+#             */
/*   Updated: 2024/10/23 12:49:03 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	msh_export(char **envp)
{
	int		i;
	int		j;
	char	*equal_sign;

	i = 0;
	while (envp[i])
	{
		equal_sign = ft_strchr(envp[i], '=');
		j = 0;
		if (equal_sign)
		{
			ft_printf("declare -x ");
			while (&envp[i][j] != equal_sign)
			{
				write(1, &envp[i][j], 1);
				j++;
			}
			ft_printf("=\"%s\"\n", equal_sign + 1);
		}
		i++;
	}
}
