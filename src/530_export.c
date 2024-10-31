/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   530_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:49:02 by luigi             #+#    #+#             */
/*   Updated: 2024/10/31 17:10:14 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	msh_export(char **envp)
{
	int		i;
	int		j;
	char	*equal_sign;

	if (!envp)
		return (1);
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
				if (write(1, &envp[i][j], 1) == -1)
					return (1);
				j++;
			}
			ft_printf("=\"%s\"\n", equal_sign + 1);
		}
		i++;
	}
	return (0);
}
