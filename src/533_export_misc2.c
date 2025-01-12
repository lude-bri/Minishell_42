/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   533_export_misc2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 16:43:42 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/12 12:01:02 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	sort_envp(char **envp)
{
	int		i;
	int		swap;
	char	*temp;

	if (!envp)
		return (1);
	swap = 1;
	while (swap)
	{
		swap = 0;
		i = 0;
		while (envp[i] && envp[i + 1])
		{
			if (ft_strcmp(envp[i], envp[i + 1]) > 0)
			{
				temp = envp[i];
				envp[i] = envp[i + 1];
				envp[i + 1] = temp;
				swap = 1;
			}
			i++;
		}
	}
	return (0);
}

int	sanity_check_export(const char *var)
{
	int		i;

	i = 0;
	if (ft_isdigit(var[0]))
		return (FAILURE);
	while (var[i])
	{
		if (var[i] == '.' || var[i] == '/')
			return (FAILURE);
		else
			i++;
	}
	return (SUCCESS);
}

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
