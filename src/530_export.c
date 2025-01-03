/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   530_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:49:02 by luigi             #+#    #+#             */
/*   Updated: 2024/12/23 01:24:34 by mde-agui         ###   ########.fr       */
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
	free(exp->var);
	return (0);
}

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
	if (is_alpha(new_var))
		return (1);
	exp->len = ft_strlen(new_var);
	exp->no_sign = ft_strnstr(new_var, "", exp->len);
	exp->equal_sign = ft_strnstr(new_var, "=", exp->len);
	exp->add_sign = ft_strnstr(new_var, "+=", exp->len);
	exp->remove_sign = ft_strnstr(new_var, "-=", exp->len);
	if (exp->add_sign)
		exp->var = ft_substr(new_var, 0, exp->add_sign - new_var);
	else if (exp->remove_sign)
		exp->var = ft_substr(new_var, 0, exp->remove_sign - new_var);
	else if (exp->equal_sign)
		exp->var = ft_substr(new_var, 0, exp->equal_sign - new_var);
	else if (exp->no_sign)
		exp->var = ft_substr(new_var, 0, exp->no_sign - new_var);
	else
		return (1);
	return (exp->var == NULL);
}

int	msh_export(t_msh *msh, char ***envp, const char *new_var)
{
	t_exp	exp;
	int		update_var;

	(void)envp;
	if (parse_variable(new_var, &exp))
	{
		perror(" not a valid identifier\n");
		return (1);
	}
	update_var = update_existing_variable(&msh->envp, &exp);
	if (update_var == 1)
		return (1);
	if (update_var == 0)
		return (0);
	if (add_new_variable(&msh->ex_envp, &exp, new_var))
		return (1);
	if (exp.equal_sign)
		add_new_variable_env(&msh->envp, &exp, new_var);
	return (0);
}
