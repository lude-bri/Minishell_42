/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   530_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:49:02 by luigi             #+#    #+#             */
/*   Updated: 2024/11/11 19:09:18 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	msh_export_no_var(char **envp)
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

int	msh_export(char ***envp, const char *new_var)
{
	int		i;
	char	*equal_sign;
	char	*var_name;
	char	*new_var_entry;
	char	**new_envp;

	i = 0;
	equal_sign = ft_strchr(new_var, '=');
	if (!equal_sign)
		return (1);
	var_name = ft_substr(new_var, 0, equal_sign - new_var);
	if (!var_name)
		return (1);
	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], var_name, ft_strlen(var_name)) == 0 && \
		(*envp)[i][ft_strlen(var_name)] == '=')
		{
			free((*envp)[i]);
			(*envp)[i] = ft_strdup(new_var);
			free(var_name);
			if ((*envp)[i] == NULL)
				return (1);
			else
				return (0);
		}
		i++;
	}
	new_envp = malloc(sizeof(char *) * (i + 2));
	if (!new_envp)
	{
		free(var_name);
		return (1);
	}
	i = 0;
	while ((*envp)[i])
	{
		new_envp[i] = (*envp)[i];
		i++;
	}
	new_var_entry = ft_strdup(new_var);
	if (!new_var_entry)
	{
		free(new_envp);
		free(var_name);
		return (1);
	}
	new_envp[i] = new_var_entry;
	new_envp[i + 1] = NULL;
	free(*envp);
	*envp = new_envp;
	return (0);
}
