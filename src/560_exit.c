/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   560_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:48:51 by luigi             #+#    #+#             */
/*   Updated: 2024/10/31 16:50:04 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_num(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	is_letter(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isalpha(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	msh_exit(char **argv, t_msh *msh)
{
	if (argv[2])
	{
		if (is_num(argv[2]))
		{
			printf("msh: exit: too many arguments\n");
			msh->exit_status = 1;
		}
	}
	if (!is_num(argv[2]) && (is_letter(argv[1]) == 0 || is_letter(argv[2]) == 0))
	{
		printf("msh: exit: numeric arguments required\n");
		msh->exit_status = 2;
	}
	else if (argv[1] && is_num(argv[1]) == 0)
	{	
		if (is_num(argv[1]))
			msh->exit_status = ft_atoi(argv[1]);
		else
		{
			printf("Minishell: exit: %s:", argv[1]);
			ft_putstr_fd("numeric argument necessary\n", 2);
			msh->exit_status = 255;
		}
	}
	free_arg(argv);	
	free_array(msh->envp, 0);
	free(msh->cmds);
	printf("exit\n");
	exit (msh->exit_status);
}
