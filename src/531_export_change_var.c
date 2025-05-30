/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   531_export_change_var.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 01:05:22 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/12 11:59:22 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Adds a new variable entry to the environment array.
 *
 * Allocates a new environment array, copies the old values, adds the new one,
 * and replaces the old array with the new one.
 *
 * @param envp Pointer to the environment array.
 * @param exp Pointer to the export context (`t_exp`).
 * @param new_var The full string to add (e.g., "FOO=bar").
 * @return 0 on success; 1 on failure.
 */
int	add_new_variable(char ***envp, t_exp *exp, const char *new_var)
{
	int	i;

	i = 0;
	while ((*envp)[i])
		i++;
	exp->new_envp = malloc(sizeof(char *) * (i + 2));
	if (!exp->new_envp)
		return (free(exp->var), 1);
	i = 0;
	while ((*envp)[i])
	{
		exp->new_envp[i] = (*envp)[i];
		i++;
	}
	exp->new_entry = ft_strdup(new_var);
	if (!exp->new_entry)
		return (free(exp->new_envp), free(exp->var), 1);
	exp->new_envp[i] = exp->new_entry;
	exp->new_envp[i + 1] = NULL;
	free(*envp);
	*envp = exp->new_envp;
	return (0);
}

/**
 * @brief Handles `+=` syntax by appending a value to the existing one.
 *
 * - Extracts the portion after `+=` and appends it to the current variable value.
 * - Stores the result in `exp->updated_value`.
 *
 * @param exp Pointer to the export context (`t_exp`).
 * @return 0 on success; 1 on allocation failure.
 */
int	handle_addition(t_exp *exp)
{
	exp->to_add = exp->add_sign + 2;
	exp->updated_value = malloc(ft_strlen(exp->existing_value)
			+ ft_strlen(exp->to_add) + 1);
	if (!exp->updated_value)
		return (1);
	ft_strlcpy(exp->updated_value, exp->existing_value,
		ft_strlen(exp->existing_value) + 1);
	ft_strncat(exp->updated_value, exp->to_add, ft_strlen(exp->to_add) + 1);
	return (0);
}

/**
 * @brief Handles `-=` syntax by removing a substring from the existing value.
 *
 * If the substring to remove is found, it constructs a new value without it.
 * If not found, keeps the original value.
 *
 * @param exp Pointer to the export context (`t_exp`).
 * @return 0 on success; 1 on allocation failure.
 */
int	handle_removal(t_exp *exp)
{
	exp->to_remove = exp->remove_sign + 2;
	exp->found = ft_strnstr(exp->existing_value, exp->to_remove,
			ft_strlen(exp->existing_value));
	if (exp->found)
	{
		exp->prefix_len = exp->found - exp->existing_value;
		exp->updated_value = malloc(ft_strlen(exp->existing_value)
				- ft_strlen(exp->to_remove) + 1);
		if (!exp->updated_value)
			return (1);
		ft_strlcpy(exp->updated_value, exp->existing_value,
			exp->prefix_len + 1);
		ft_strncat(exp->updated_value, exp->found + ft_strlen(exp->to_remove),
			ft_strlen(exp->existing_value) - (exp->prefix_len
				+ ft_strlen(exp->to_remove)) + 1);
	}
	else
		exp->updated_value = ft_strdup(exp->existing_value);
	return (0);
}

/**
 * @brief Updates a variable in the environment with a new value.
 *
 * - Builds the new entry string (`VAR=value` or just `VAR`)
 * - Replaces the old entry in `envp[i]`
 * - Frees old memory where needed
 *
 * @param envp Pointer to the environment array.
 * @param exp Double pointer to the export context.
 * @param i Index of the variable to update.
 * @return 0 on success; 1 on allocation failure.
 */
int	update_variable_entry(char ***envp, t_exp **exp, int i)
{
	if ((*exp)->equal_sign)
	{
		(*exp)->new_entry = malloc(ft_strlen((*exp)->var)
				+ ft_strlen((*exp)->updated_value) + 2);
		if (!(*exp)->new_entry)
			return (1);
		ft_strlcpy((*exp)->new_entry, (*exp)->var, ft_strlen((*exp)->var) + 1);
		ft_strncat((*exp)->new_entry, "=", 2);
		ft_strncat((*exp)->new_entry, (*exp)->updated_value,
			ft_strlen((*exp)->updated_value) + 1);
	}
	else
	{
		(*exp)->new_entry = ft_strdup((*exp)->var);
		if (!(*exp)->new_entry)
			return (1);
	}
	free((*envp)[i]);
	(*envp)[i] = (*exp)->new_entry;
	if ((*exp)->updated_value)
		free((*exp)->updated_value);
	return (0);
}

/**
 * @brief Updates an existing environment variable if it exists.
 *
 * - Supports value replacement (`=`), addition (`+=`), and removal (`-=`)
 * - Uses `update_variable_entry()` to commit the final value
 *
 * @param envp Pointer to the environment array.
 * @param exp Pointer to the export context (`t_exp`).
 * @return 0 on success; 1 on error; 2 if the variable was not found.
 */
int	update_existing_variable(char ***envp, t_exp *exp)
{
	int	i;

	exp->flag = true;
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
