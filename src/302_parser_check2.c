/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   302_parser_check2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 16:58:16 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/07 16:59:53 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_echo_command(char *line)
{
	if ((ft_strncmp(line, "echo \"", 6) == 0)
		|| ft_strncmp(line, "echo \'", 6) == 0)
		return (SUCCESS);
	return (FAILURE);
}

int	check_operators(t_msh *msh, char *line, int *i)
{
	if (is_operator(&line[*i]) == FAILURE)
	{
		(*i)++;
		return (SUCCESS);
	}
	if ((ft_strncmp(&line[*i], "<<", 2) == 0)
		|| (ft_strncmp(&line[*i], ">>", 2) == 0))
	{
		*i += 2;
		if (is_operator(&line[*i]) == SUCCESS)
			return (error_syntax(msh), FAILURE);
	}
	if (line[*i] && line[*i + 1])
	{
		if (is_operator(&line[*i + 1]) == SUCCESS)
			return (error_syntax(msh), FAILURE);
		(*i)++;
	}
	return (SUCCESS);
}

int	check_line_syntax(t_msh *msh, char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (check_echo_command(line) == SUCCESS)
			return (SUCCESS);
		if (check_operators(msh, line, &i) == FAILURE)
			return (FAILURE);
	}
	return (SUCCESS);
}
