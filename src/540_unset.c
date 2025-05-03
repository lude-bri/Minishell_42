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

/**
 * @brief Checks if an environment variable matches a given variable name.
 *
 * A match occurs if the prefix matches and is followed by `=` or null terminator.
 *
 * @param env_var The full environment string (e.g. "PATH=/usr/bin").
 * @param var_name The variable name to match (e.g. "PATH").
 * @return 1 if it's a match; 0 otherwise.
 */
int	is_variable_match(const char *env_var, const char *var_name)
{
	int	len;

	len = ft_strlen(var_name);
	return (ft_strncmp(env_var, var_name, len) == 0 && (env_var[len] == '='
			|| env_var[len] == '\0'));
}

/**
 * @brief Removes a variable from the environment array.
 *
 * If a match is found, it frees the string and shifts remaining elements.
 *
 * @param envp Pointer to the environment variable array.
 * @param var The name of the variable to remove.
 * @return 1 if a variable was removed; 0 if not found.
 */
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
			free((*envp)[j]);
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

/**
 * @brief Verifies if an invalid option was passed to `unset`.
 *
 * Returns an error if the first argument after `unset` starts with `-`.
 *
 * @param av Argument vector (may include earlier tokens).
 * @return 1 if an invalid option is detected; 0 otherwise.
 */
static int	verify_unset_options(char **av)
{
	while (ft_strcmp(*av, "unset") != 0)
		av++;
	av++;
	if (ft_strncmp(*av, "-", 1) == 0)
		return (1);
	return (0);
}

/**
 * @brief Executes the `unset` builtin command.
 *
 * - Validates arguments and options.
 * - Removes all matching variables from `envp`.
 * - Operates in-place and handles memory cleanup.
 *
 * @param argv The full argument list (`argv[0] = "unset"`).
 * @param envp Double pointer to the environment array.
 * @return 0 on success; 1 on failure or no variable found.
 */
int	msh_unset(char **argv, char ***envp)
{
	int	i;
	int	found;

	found = 0;
	if (!argv || !envp || !*envp)
		return (1);
	found = verify_unset_options(argv);
	if (found == 1)
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
