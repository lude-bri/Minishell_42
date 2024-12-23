/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   500_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:48:34 by luigi             #+#    #+#             */
/*   Updated: 2024/12/23 00:27:21 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	verify_whitespaces(char *line)
{
	int		i;

	if (ft_strncmp("echo ", line, 5) == 0)
	{
		i = 5;
		while (line[i])
		{
			if (is_whitespace(line[i]))
				return (SUCCESS);
			i++;
		}
		return (FAILURE);
	}
	return (FAILURE);
}

int	handle_echo_options(char **argv, int *newline)
{
	int	i;

	i = 1;
	*newline = 1;
	if (argv[1] && ft_strncmp(argv[1], "-n", 2) == 0)
	{
		*newline = 0;
		i = 2;
	}
	if (ft_strcmp(argv[0], "echo") != 0)
		i++;
	return (i);
}

void	print_echo_arguments(char **argv, int i, int space)
{
	while (argv[i])
	{
		printf("%s", argv[i]);
		if (argv[i + 1] && space == 1)
			printf(" ");
		i++;
	}
}

int	msh_echo(char **argv, t_msh *msh, t_tkn *tokens)
{
	int	i;
	int	newline;
	int	space;

	(void)tokens;
	if (!argv[1])
		return (printf("\n"), 0);
	i = handle_echo_options(argv, &newline);
	if (verify_whitespaces(msh->line) == SUCCESS)
		space = 1;
	else
		space = 0;
	print_echo_arguments(argv, i, space);
	if (newline)
		printf("\n");
	return (0);
}
