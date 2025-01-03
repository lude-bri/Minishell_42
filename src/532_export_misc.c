/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   532_export_misc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 01:07:26 by mde-agui          #+#    #+#             */
/*   Updated: 2024/12/23 03:20:43 by mde-agui         ###   ########.fr       */
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

int	find_existing_variable(char ***envp, t_exp *exp)
{
	int	i;

	i = 0;
	while ((*envp)[i])
	{
		if (ft_strncmp((*envp)[i], exp->var, ft_strlen(exp->var)) == 0 &&
			(*envp)[i][ft_strlen(exp->var)] == '=')
		{
			exp->existing_value = ft_strchr((*envp)[i], '=') + 1;
			return (i);
		}
		i++;
	}
	return (-1);
}

int	is_alpha(const char *str)
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
		else if (str[i] == '+' || str[i] == '=' || (str[i] == '-'
				&& str[i + 1] == '='))
			break ;
		else
			return (1);
	}
	return (0);
}
