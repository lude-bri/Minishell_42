/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   532_export_misc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 01:07:26 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/12 12:00:24 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Prints an environment variable that contains an '=' sign.
 *
 * Formats the output as: `declare -x VAR="value"` to match Bash style.
 *
 * @param env_var The full environment variable string.
 * @param equal_sign Pointer to the '=' character inside `env_var`.
 */
static void	handle_w_equal_sign(char *env_var, char *equal_sign)
{
	int		j;

	j = 0;
	write(1, "declare -x ", 11);
	while (env_var[j] != '=')
	{
		if (write(1, &env_var[j], 1) == -1)
			return ;
		j++;
	}
	printf("=\"%s\"\n", equal_sign + 1);
}

/**
 * @brief Prints an environment variable in a shell-compatible format.
 *
 * - If the variable contains `=`, it prints `declare -x VAR="value"`.
 * - Otherwise, it prints just `declare -x VAR`.
 *
 * @param env_var The environment variable string.
 */
void	print_env_var(char *env_var)
{
	char	*equal_sign;
	int		j;

	equal_sign = ft_strchr(env_var, '=');
	if (equal_sign)
		handle_w_equal_sign(env_var, equal_sign);
	else
	{
		j = 0;
		write(1, "declare -x ", 11);
		while (env_var[j])
		{
			if (write(1, &env_var[j], 1) == -1)
				return ;
			j++;
		}
		write(1, "\n", 1);
	}
}

/**
 * @brief Finds the index of an existing variable in `envp`.
 *
 * - Matches by variable name, stopping at `=` or string end.
 * - Updates `exp->existing_value` if a match is found and has `=`.
 *
 * @param envp Pointer to the environment variable array.
 * @param exp Double pointer to the export context (`t_exp`).
 * @return Index of the variable if found, -1 otherwise.
 */
int	find_existing_variable(char ***envp, t_exp **exp)
{
	int		i;
	size_t	var_len;
	char	*equal_pos;

	i = 0;
	var_len = ft_strlen((*exp)->var);
	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], (*exp)->var, var_len) == 0
			&& ((*envp)[i][var_len] == '\0' || (*envp)[i][var_len] == '='))
		{
			equal_pos = ft_strchr((*envp)[i], '=');
			if (equal_pos)
				(*exp)->existing_value = equal_pos + 1;
			else
				(*exp)->existing_value = NULL;
			return (i);
		}
		i++;
	}
	return (-1);
}

/**
 * @brief Checks if a variable exists in the environment.
 *
 * Used when no assignment is given in the export command.
 *
 * @param envp Pointer to the environment array.
 * @param var Variable name (without '=' or value).
 * @return 1 if found; 0 otherwise.
 */
int	variable_exists(char ***envp, const char *var)
{
	int		i;
	size_t	var_len;

	i = 0;
	var_len = ft_strlen(var);
	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], var, var_len) == 0 &&
			((*envp)[i][var_len] == '\0' || (*envp)[i][var_len] == '='))
		{
			return (1);
		}
		i++;
	}
	return (0);
}

/**
 * @brief Updates an existing variable in the export environment (`ex_envp`).
 *
 * Supports:
 * - Appending (`+=`)
 * - Removing substrings (`-=`)
 * - Replacing (`=`)
 *
 * Calls `update_variable_entry()` to finalize the update.
 *
 * @param envp Pointer to the environment array.
 * @param exp Pointer to the export context (`t_exp`).
 * @return 0 on success; 1 on memory error; 2 if variable not found.
 */
int	update_existing_variable_env(char ***envp, t_exp *exp)
{
	int	i;

	exp->flag = false;
	i = find_existing_variable(envp, &exp);
	if (i == -1)
		return (2);
	if (exp->add_sign)
	{
		if (handle_addition(exp))
			return (free(exp->var), 1);
	}
	else if (exp->remove_sign)
	{
		if (handle_removal(exp))
			return (free(exp->var), 1);
	}
	else
	{
		exp->updated_value = ft_strdup(exp->equal_sign + 1);
		if (!exp->updated_value)
			return (free(exp->var), 1);
	}
	if (update_variable_entry(envp, &exp, i))
		return (1);
	return (0);
}
