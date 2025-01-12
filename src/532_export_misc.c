/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   532_export_misc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 01:07:26 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/12 10:51:29 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

// Update find_existing_variable in export_misc.c
int find_existing_variable(char ***envp, t_exp **exp)
{
    int i;
    size_t var_len;
    char *equal_pos;

    i = 0;
    var_len = ft_strlen((*exp)->var);
    while ((*envp)[i])
    {
        // Match either exact variable name or variable with =
        if (ft_strncmp((*envp)[i], (*exp)->var, var_len) == 0 && 
            ((*envp)[i][var_len] == '\0' || (*envp)[i][var_len] == '='))
        {
            // If variable has a value, store it
            equal_pos = ft_strchr((*envp)[i], '=');
            if (equal_pos)
                (*exp)->existing_value = equal_pos + 1;
            else
                (*exp)->existing_value = NULL;
            return i;
        }
        i++;
    }
    return -1;
}

int	variable_exists(char ***envp, const char *var)
{
	int		i;
	size_t	var_len;

	i = 0;
	var_len = ft_strlen(var);
	while ((*envp)[i])
	{
		printf("Comparing: '%s' with '%s'\n", (*envp)[i], var);
		if (ft_strncmp((*envp)[i], var, var_len) == 0 &&
			((*envp)[i][var_len] == '\0' || (*envp)[i][var_len] == '='))
		{
			printf("Match found!\n");
			return (1);
		}
		i++;
	}
	printf("No match found for '%s'\n", var);
	return (0);
}

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
