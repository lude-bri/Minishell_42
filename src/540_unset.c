/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   540_unset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/26 12:49:07 by luigi             #+#    #+#             */
/*   Updated: 2024/12/23 01:31:15 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_variable_match(const char *env_var, const char *var_name)
{
	int	len;

	len = ft_strlen(var_name);
	return (ft_strncmp(env_var, var_name, len) == 0 && (env_var[len] == '='
			|| env_var[len] == '\0'));
}

int	remove_variable(char ***envp, char *var)
{
	int	j;
	int	found;

	j = 0;
	found = 0;
	while ((*envp)[j])
	{
		if (is_variable_match((*envp)[j], var))
		{
			found = 1;
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
	return (found);
}

int	msh_unset(char **argv, char ***envp)
{
	int	i;
	int	found;

	found = 0;
	if (!argv || !envp || !*envp)
		return (1);
	i = 1;
	while (argv[i])
	{
		if (ft_strlen(argv[i]) > 0)
		{
			if (remove_variable(envp, argv[i]))
				found = 1;
		}
		i++;
	}
	if (found)
		return (0);
	else
		return (1);
}
