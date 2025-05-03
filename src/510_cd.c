/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   510_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:48:25 by luigi             #+#    #+#             */
/*   Updated: 2024/12/23 00:37:28 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Finds the index of an environment variable in the envp array.
 *
 * Compares the prefix of each environment string with the target variable name.
 *
 * @param envp The environment variable array.
 * @param var The variable name to search for (e.g., "PWD").
 * @return The index of the variable if found; -1 otherwise.
 */
int	find_env_var(char **envp, const char *var)
{
	int		i;
	size_t	var_len;

	i = 0;
	var_len = ft_strlen(var);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], var, var_len) == 0 && envp[i][var_len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

/**
 * @brief Updates or replaces an environment variable in the envp array.
 *
 * Constructs a new string of the format `VAR=value`, frees the previous
 * value (if found), and replaces it in-place.
 *
 * @param envp The environment variable array.
 * @param var The variable name (e.g., "PWD" or "OLDPWD").
 * @param value The new value to assign to the variable.
 */
void	update_env_var(char **envp, const char *var, const char *value)
{
	int		index;
	char	*new_var;
	size_t	var_len;
	size_t	value_len;

	var_len = ft_strlen(var);
	value_len = ft_strlen(value);
	new_var = malloc(var_len + value_len + 2);
	if (!new_var)
		return ;
	ft_strlcpy(new_var, var, var_len + 1);
	ft_strlcat(new_var, "=", var_len + 2);
	ft_strlcat(new_var, value, var_len + value_len + 2);
	index = find_env_var(envp, var);
	if (index != -1)
	{
		free(envp[index]);
		envp[index] = new_var;
	}
}

/**
 * @brief Implements the `cd` builtin command.
 *
 * Handles the following:
 * - `cd` with no arguments: goes to `$HOME`
 * - `cd [dir]`: changes to the given directory
 * - Too many arguments: prints an error
 * - Updates `PWD` and `OLDPWD` after a successful change
 *
 * @param argv Command arguments (`argv[0] = "cd"`, `argv[1] = target`)
 * @param envp The environment variable array (modifiable).
 * @return 0 on success; 1 on failure (with appropriate error messages).
 */
int	msh_cd(char **argv, char **envp)
{
	char	current_path[PATH_MAX];
	char	new_path[PATH_MAX];
	char	*home;

	if (!argv || !envp)
		return (1);
	if (getcwd(current_path, sizeof(current_path)) == NULL)
		return (perror("msh: getcwd"), 1);
	if (argv[1] == NULL)
	{
		home = getenv("HOME");
		if (!home)
			return (printf("msh: cd: HOME not set\n"), 1);
		if (chdir(home) != 0)
			return (perror("msh: cd"), 1);
	}
	else if (argv[2])
		return (write(STDERR_FILENO, " too many arguments\n", 20), 1);
	else if (chdir(argv[1]) != 0)
		return (perror("msh: cd"), 1);
	if (getcwd(new_path, sizeof(new_path)) == NULL)
		return (perror("msh: getcwd"), 1);
	update_env_var(envp, "OLDPWD", current_path);
	update_env_var(envp, "PWD", new_path);
	return (0);
}
