/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   540_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 12:49:07 by luigi             #+#    #+#             */
/*   Updated: 2024/10/26 12:49:09 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_variable_match(const char *env_var, const char *var_name)
{
	int	len;

	len = ft_strlen(var_name);
	return (ft_strncmp(env_var, var_name, len) == 0 && env_var[len] == '=');
}

void	msh_unset(char **argv, char ***envp)
{
	int	i;
	int	j;

	i = 1;
	while (argv[i])
	{
		if (ft_strlen(argv[i]) > 0)
		{
			j = 0;
			while ((*envp)[j])
			{
				if (is_variable_match((*envp)[j], argv[i]))
				{
					//free((*envp)[j]);
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
}
