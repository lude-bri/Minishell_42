/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   500_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:48:34 by luigi             #+#    #+#             */
/*   Updated: 2024/10/31 17:09:27 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	msh_echo(char **argv)
{
	int	i;
	int	newline;

	if (!argv[1])
		ft_printf("\n");
	i = 1;
	newline = 1;
	if (argv[1] && ft_strncmp(argv[1], "-n", 2) == 0)
	{
		newline = 0;
		i = 2;
	}
	while (argv[i])
	{
		ft_printf("%s", argv[i]);
		if (argv[i + 1])
			ft_printf(" ");
		i++;
	}
	if (newline)
		ft_printf("\n");
	return (0);
}
