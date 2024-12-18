/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   530_export.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:49:02 by luigi             #+#    #+#             */
/*   Updated: 2024/12/14 19:33:52 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	int		i;
	int		j;
	char	*equal_sign;

	if (!envp)
		return (1);
	if (sort_envp(envp))
		return (1);
	i = 0;
	while (envp[i])
	{
		equal_sign = ft_strchr(envp[i], '=');
		j = 0;
		if (equal_sign)
		{
			ft_printf("declare -x ");
			while (&envp[i][j] != equal_sign)
			{
				if (write(1, &envp[i][j], 1) == -1)
					return (1);
				j++;
			}
			ft_printf("=\"%s\"\n", equal_sign + 1);
		}
		i++;
	}
	return (0);
}

int	add_new_variable(char ***envp, t_exp *exp, const char *new_var)
{
	int i;

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

int	update_existing_variable(char ***envp, t_exp *exp)
{
	int i;

	i = 0;
	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], exp->var, ft_strlen(exp->var)) == 0 &&
			(*envp)[i][ft_strlen(exp->var)] == '=')
		{
			exp->existing_value = ft_strchr((*envp)[i], '=') + 1;
			if (exp->add_sign)
			{
				exp->to_add = exp->add_sign + 2;
				exp->updated_value = malloc(ft_strlen(exp->existing_value) +
					ft_strlen(exp->to_add) + 1);
				if (!exp->updated_value)
					return (free(exp->var), 1);
				ft_strlcpy(exp->updated_value, exp->existing_value, ft_strlen(exp->existing_value) + 1);
				ft_strncat(exp->updated_value, exp->to_add, ft_strlen(exp->to_add) + 1);
			}
			else if (exp->remove_sign)
			{
				exp->to_remove = exp->remove_sign + 2;
				exp->found = ft_strnstr(exp->existing_value, exp->to_remove, ft_strlen(exp->existing_value));
				if (exp->found)
				{
					exp->prefix_len = exp->found - exp->existing_value;
					exp->updated_value = malloc(ft_strlen(exp->existing_value) - ft_strlen(exp->to_remove) + 1);
					if (!exp->updated_value)
						return (free(exp->var), 1);
					ft_strlcpy(exp->updated_value, exp->existing_value, exp->prefix_len + 1);
					ft_strncat(exp->updated_value, exp->found + ft_strlen(exp->to_remove),
						ft_strlen(exp->existing_value) - (exp->prefix_len + ft_strlen(exp->to_remove)) + 1);
				}
				else
					exp->updated_value = ft_strdup(exp->existing_value); // No change
			}
			else
				exp->updated_value = ft_strdup(exp->equal_sign + 1);
			if (!exp->updated_value)
				return (free(exp->var), 1);
			exp->new_entry = malloc(ft_strlen(exp->var) + ft_strlen(exp->updated_value) + 2);
			if (!exp->new_entry)
				return (free(exp->updated_value), free(exp->var), 1);
			ft_strlcpy(exp->new_entry, exp->var, ft_strlen(exp->var) + 1);
			ft_strncat(exp->new_entry, "=", 2);
			ft_strncat(exp->new_entry, exp->updated_value, ft_strlen(exp->updated_value) + 1);
			free((*envp)[i]);
			(*envp)[i] = exp->new_entry;
			free(exp->updated_value);
			free(exp->var);
			return (0);
		}
		i++;
	}
	return (2);
}

static int	is_alpha(const char *str)
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
		else if (str[i] == '+' || str[i] == '=' || (str[i] == '-' && str[i + 1] == '='))
			break ;
		else
			return (1);
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

int	msh_export(char ***envp, const char *new_var)
{
	t_exp	exp;
	int		update_var;

	if (parse_variable(new_var, &exp))
	{
		perror(" not a valid identifier\n");
		return (1);
	}
	update_var = update_existing_variable(envp, &exp);
	if (update_var == 1)
		return (1);
	if (update_var == 0)
		return (0);
	if (add_new_variable(envp, &exp, new_var))
		return (1);
	return (0);
}
