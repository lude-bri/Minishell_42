/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   111_init_var.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:55:08 by luigi             #+#    #+#             */
/*   Updated: 2024/10/26 12:49:49 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char		*find_variable(char *target, char **envp);

/**
 * @brief Increments the SHLVL environment variable by 1.
 *
 * Searches through the environment variable array for the "SHLVL" entry,
 * increments its numeric value, and updates it in-place.
 * If not found, adds "SHLVL=1" to the environment.
 *
 * @param envp Array of strings representing the environment variables.
 */
void	increment_shlvl(char **envp)
{
	int			i;
	int			shlvl_value;
	char		*new_shlvl;
	char		*shlvl_str;

	i = 0;
	while (envp[i])
	{
		if (ft_strnstr(envp[i], "SHLVL=", 6))
		{
			shlvl_value = ft_atoi(envp[i] + 6);
			shlvl_value++;
			shlvl_str = ft_itoa(shlvl_value);
			new_shlvl = ft_strjoin("SHLVL=", shlvl_str);
			free(envp[i]);
			envp[i] = new_shlvl;
			free(shlvl_str);
			return ;
		}
		i++;
	}
	envp[i] = ft_strdup("SHLVL=1");
	envp[i + 1] = NULL;
}


/**
 * @brief Retrieves the value of a specific environment variable.
 *
 * Delegates the actual search to find_variable().
 *
 * @param v Name of the variable to search for (e.g., "PATH").
 * @param envp Array of strings representing the environment variables.
 * @return A newly allocated string with the variable's value, or NULL if not found.
 */
char	*get_variable(char *v, char **envp)
{
	char	*find;

	find = find_variable(v, envp);
	return (find);
}


/**
 * @brief Searches for an environment variable and extracts its value.
 *
 * Constructs the search pattern "VAR=" and compares against each entry in envp.
 * If found, returns a newly allocated string containing the value (after the '=').
 *
 * @param target Name of the variable to find (e.g., "HOME").
 * @param envp Array of strings representing the environment variables.
 * @return A newly allocated string with the variable's value, or NULL if not found.
 */
static char	*find_variable(char *target, char **envp)
{
	char	*tmp;
	char	*var;
	int		len;

	if ((target == NULL) || (envp == NULL))
		return (NULL);
	tmp = ft_strjoin(target, "=");
	len = ft_strlen(tmp);
	var = NULL;
	while (*envp)
	{
		if (ft_strncmp(tmp, *envp, len) == 0)
		{
			var = ft_strdup(*envp + len);
			break ;
		}
		++envp;
	}
	free(tmp);
	return (var);
}
