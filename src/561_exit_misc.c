/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   561_exit_misc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 01:38:29 by mde-agui          #+#    #+#             */
/*   Updated: 2024/12/23 01:41:15 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_num(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	is_letter(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_isalpha(str[i]))
			return (1);
		i++;
	}
	return (0);
}

int	is_sign(char *argv)
{
	return ((ft_strncmp(argv, "+", 1) == 0) || (ft_strncmp(argv, "-", 1) == 0));
}
