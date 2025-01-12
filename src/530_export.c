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
