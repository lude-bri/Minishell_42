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

/**
 * @brief Sorts the environment variable array in lexicographical order.
 *
 * Uses a basic bubble sort to order strings in-place.
 *
 * @param envp The array of environment variables.
 * @return 0 on success; 1 if `envp` is NULL.
 */
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

/**
 * @brief Validates that a variable name is suitable for export.
 *
 * Rejects names that:
 * - Start with a digit
 * - Contain '.' or '/'
 *
 * @param var The variable string to check (e.g. `"1FOO"` or `"FOO.bar"`).
 * @return SUCCESS if valid; FAILURE otherwise.
 */
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

/**
 * @brief Checks if a string contains only alphabetical characters.
 *
 * Stops at operators (`+`, `=`, `-=`). Returns failure if any other
 * invalid character is encountered.
 *
 * @param str The string to check.
 * @return 0 if valid; 1 if it contains non-alphabetic characters or starts with '='.
 */
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
