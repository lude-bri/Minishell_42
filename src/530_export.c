/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   530_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:49:02 by luigi             #+#    #+#             */
/*   Updated: 2025/01/12 11:57:04 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Adds a new variable to the environment array.
 *
 * Creates a new envp array one element larger, copies existing variables,
 * adds the new variable, and frees the old array.
 *
 * @param envp Pointer to the environment array.
 * @param exp Pointer to the t_exp helper structure.
 * @param new_var The variable string to add (e.g., "VAR=value").
 * @return 0 on success; 1 on allocation or copy failure.
 */
static int	add_new_variable_env(char ***envp, t_exp *exp, const char *new_var)
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
 * @brief Handles the `export` command when no arguments are provided.
 *
 * - Sorts and prints the environment variables in export format.
 * - Excludes non-exported values (i.e., `envp` vs `ex_envp` logic).
 *
 * @param envp The environment variable array.
 * @return 0 on success; 1 on failure.
 */
int	msh_export_no_var(char **envp)
{
	int	i;

	if (!envp)
		return (1);
	if (sort_envp(envp))
		return (1);
	i = 0;
	while (envp[i])
	{
		print_env_var(envp[i]);
		i++;
	}
	return (0);
}

/**
 * @brief Parses a new variable string and extracts relevant parts.
 *
 * Handles syntax like:
 * - `VAR=val`
 * - `VAR+=val`
 * - `VAR-=val`
 * - `VAR`
 *
 * Saves the variable name into `exp->var` and tracks whether it's an update.
 *
 * @param new_var The full input string to parse.
 * @param exp Pointer to the `t_exp` structure to populate.
 * @return 0 on success; 1 on failure or invalid syntax.
 */
int	parse_variable(const char *new_var, t_exp *exp)
{
	if (sanity_check_export(new_var) == FAILURE)
		return (1);
	exp->flag = false;
	exp->len = ft_strlen(new_var);
	exp->equal_sign = ft_strnstr(new_var, "=", exp->len);
	exp->add_sign = ft_strnstr(new_var, "+=", exp->len);
	exp->remove_sign = ft_strnstr(new_var, "-=", exp->len);
	if (exp->add_sign)
		exp->var = ft_substr(new_var, 0, exp->add_sign - new_var);
	else if (exp->remove_sign)
		exp->var = ft_substr(new_var, 0, exp->remove_sign - new_var);
	else if (exp->equal_sign)
		exp->var = ft_substr(new_var, 0, exp->equal_sign - new_var);
	else
		exp->var = ft_strdup(new_var);
	return (exp->var == NULL);
}

/**
 * @brief Handles updating or appending to an existing variable.
 *
 * - Attempts to update `envp` and `ex_envp` depending on `exp`.
 * - Falls back to `add_new_variable_env()` if the variable doesn't exist.
 *
 * @param msh Pointer to the shell state.
 * @param exp Pointer to the parsed export struct.
 * @param new_var The original variable input string.
 * @return 0 on success; 1 on failure.
 */
int	handle_variable_update(t_msh *msh, t_exp *exp, const char *new_var)
{
	int	update_var;
	int	update_var_ex;

	update_var_ex = update_existing_variable_env(&msh->ex_envp, exp);
	if (update_var_ex == 1)
		return (free(exp->var), 1);
	if (exp->equal_sign)
	{
		update_var = update_existing_variable(&msh->envp, exp);
		if (update_var == 1)
			return (free(exp->var), 1);
		if (update_var == 2)
		{
			if (add_new_variable_env(&msh->envp, exp, new_var))
				return (free(exp->var), 1);
		}
	}
	if (update_var_ex == 2)
	{
		if (add_new_variable(&msh->ex_envp, exp, new_var))
			return (free(exp->var), 1);
	}
	return (0);
}

/**
 * @brief Main handler for the `export` builtin command.
 *
 * - Parses the new variable and validates its format.
 * - If no assignment/operator is found, adds it only to `ex_envp`.
 * - Otherwise, tries to update both `envp` and `ex_envp`.
 *
 * @param msh Pointer to the shell state.
 * @param envp Double pointer to the environment (unused directly).
 * @param new_var The user input variable (e.g. `VAR=value`).
 * @return 0 on success; 1 on failure.
 */
int	msh_export(t_msh *msh, char ***envp, const char *new_var)
{
	t_exp	exp;

	(void)envp;
	if (parse_variable(new_var, &exp))
		return (perror(" not a valid identifier\n"), 1);
	if (!exp.equal_sign && !exp.add_sign && !exp.remove_sign)
	{
		if (!variable_exists(&msh->ex_envp, exp.var))
		{
			if (add_new_variable(&msh->ex_envp, &exp, new_var))
				return (free(exp.var), 1);
		}
		return (free(exp.var), 0);
	}
	if (handle_variable_update(msh, &exp, new_var))
		return (1);
	free(exp.var);
	return (0);
}
