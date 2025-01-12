/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   533_export_misc2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:43:42 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/11 16:44:16 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_alpha(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '=')
		return (1);
	while (str[i])
	{
		if ((str[i] >= 'a' && str[i] <= 'z')
			|| (str[i] >= 'A' && str[i] <= 'Z'))
			i++;
		else if (str[i] == '+' || str[i] == '=' || (str[i] == '-'
				&& str[i + 1] == '='))
			break ;
		else
			return (1);
	}
	return (0);
}
