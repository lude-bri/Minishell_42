/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   560_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:48:51 by luigi             #+#    #+#             */
/*   Updated: 2024/10/31 19:01:57 by mde-agui         ###   ########.fr       */
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
	if (msh->cmd_count > 2)
	{
		if (is_letter(argv[1]))
		{
			ft_printf("msh: exit: %s: numeric argument required\n", argv[1]);
			msh->exit_status = 2;
		}
		else
		{
			ft_printf("msh: exit: too many arguments\n");
			msh->exit_status = 1;
			return (msh->exit_status);
		}
	}
	if (!argv[1])
	{
		ft_printf("exit\n");
		free_arg(argv);
		free_array(msh->envp, 0);
		free(msh->cmds);
		free_vector(&msh->tokens);
		// free_tokens(tokens);
		exit(msh->exit_status);
	}
	if (is_num(argv[1]))
		msh->exit_status = ft_atoi(argv[1]);
	else if (msh->exit_status != 2)
	{
		ft_printf("msh: exit: %s: numeric argument required\n", argv[1]);
		msh->exit_status = 2;
	}
	ft_printf("exit\n");
	free_arg(argv);
	free_array(msh->envp, 0);
	free(msh->cmds);
	free_vector(&msh->tokens);
	exit(msh->exit_status);
}
