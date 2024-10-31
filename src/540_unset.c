/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   540_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 12:49:07 by luigi             #+#    #+#             */
/*   Updated: 2024/10/31 17:10:31 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_variable_match(const char *env_var, const char *var_name)
{
	int	len;

	len = ft_strlen(var_name);
	return (ft_strncmp(env_var, var_name, len) == 0 && env_var[len] == '=');
}

int	msh_unset(char **argv, char ***envp)
{
	int	i;
	int	j;
	int	found;

	if (!argv || !envp || !*envp)
		return (1);
	i = 1;
	found = 0;
	while (argv[i])
	{
		if (ft_strlen(argv[i]) > 0)
		{
			j = 0;
			while ((*envp)[j])
			{
				found = 1;
				if (is_variable_match((*envp)[j], argv[i]))
				{
					while ((*envp)[j + 1])
					{
						(*envp)[j] = (*envp)[j + 1];
						j++;
					}
					(*envp)[j] = NULL;
					break ;
				}
				j++;
			}
		}
		i++;
	}
	if (found)
		return (0);
	else
		return (1);
}
