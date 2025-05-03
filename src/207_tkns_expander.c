/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   204_tkns_expander.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 23:18:51 by mde-agui          #+#    #+#             */
/*   Updated: 2024/12/22 01:32:05 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Expands a variable and appends its value to msh->word buffer.
 *
 * Uses `expand_var()` to get the variable's value, then appends it to
 * the current word buffer in the shell structure. If the result would
 * exceed the buffer size, expansion is aborted and NULL is returned.
 *
 * @param input The input string.
 * @param i Pointer to the current index in the input; will be advanced.
 * @param msh Pointer to the main shell structure, containing `word` and `word_size`.
 * @return The updated word buffer on success, or NULL on failure (e.g., overflow).
 */
char	*expand_helper(const char *input, int *i, t_msh *msh)
{
	char	*expanded;

	expanded = expand_var(input, i, msh);
	if (ft_strlen(expanded) + ft_strlen(msh->word) >= msh->word_size - 1)
		return (free(expanded), NULL);
	ft_strncat(msh->word, expanded, msh->word_size - ft_strlen(msh->word) - 1);
	free(expanded);
	return (msh->word);
}

/**
 * @brief Extracts and expands an environment variable from the input string.
 *
 * Handles special case `$?` to return the last exit status.
 * Otherwise, reads the variable name, looks it up in the environment,
 * and returns a newly allocated string with its value.
 *
 * Advances the input index past the entire variable name.
 *
 * @param input The input string containing the `$` symbol.
 * @param i Pointer to the current index in the input string (updated after parsing).
 * @param msh Pointer to the shell structure (used to access `exit_status` and `envp`).
 * @return A newly allocated string with the expanded value, or an empty string if undefined.
 */
char	*expand_var(const char *input, int *i, t_msh *msh)
{
	int		len;
	t_exp	exp;

	len = 0;
	exp.start = *i + 1;
	if (!input[1])
		return (NULL);
	if (input[exp.start] == '?')
		return (exp.result = ft_itoa(msh->exit_status), *i += 2, exp.result);
	while (input[exp.start + len] && (ft_isalnum(input[exp.start + len])
			|| input[exp.start + len] == '_'))
		len++;
	exp.var_name = (char *)malloc(len + 1);
	if (!exp.var_name)
		return (NULL);
	ft_strlcpy(exp.var_name, &input[exp.start], len + 1);
	exp.var_name[len] = '\0';
	exp.value = get_variable(exp.var_name, msh->envp);
	free(exp.var_name);
	if (exp.value)
		exp.result = ft_strdup(exp.value);
	else
		exp.result = ft_strdup("");
	*i += len + 1;
	return (free(exp.value), exp.result);
}
