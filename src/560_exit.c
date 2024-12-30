/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   560_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:48:51 by luigi             #+#    #+#             */
/*   Updated: 2024/12/23 01:50:26 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	verify_arg(char **argv, t_msh *msh, t_tkn *tokens);
int			msh_exit(char **argv, t_msh *msh, t_tkn *tokens);
static int	exit_msh(char **argv, t_msh *msh, t_tkn *tokens);

static int	handle_two_commands(char **argv, t_msh *msh)
{
	if (is_letter(argv[1]))
		return (write(STDERR_FILENO, " numeric argument required\n", 26), 2);
	else
	{
		if (is_sign(argv[1]))
		{
			if (is_num(argv[1]) && ft_strncmp(argv[1], "+", 1) == 0)
				msh->exit_status = ft_atoi(argv[1]);
			else
				msh->exit_status = 256 + ft_atoi(argv[1]);
		}
		else
			return (ft_atoi(argv[1]));
	}
	return (msh->exit_status);
}

static int	handle_multiple_commands(char **argv, t_msh *msh)
{
	if (is_sign(argv[1]))
	{
		if (is_num(argv[2]) && ft_strncmp(argv[1], "+", 1) == 0)
			msh->exit_status = ft_atoi(argv[2]);
		else
			msh->exit_status = 256 - ft_atoi(argv[2]);
	}
	else if (is_letter(argv[1]))
	{
		msh->flag_exit = false;
		return (write(STDERR_FILENO, " numeric argument required\n", 26), 2);
	}
	else
	{
		msh->flag_exit = true;
		return (write(STDERR_FILENO, " too many arguments\n", 20), 1);
	}
	return (msh->exit_status);
}

static int	verify_arg(char **argv, t_msh *msh, t_tkn *tokens)
{
	if (!argv[1])
		msh->exit_status = exit_msh(argv, msh, tokens);
	if (msh->cmd_count == 2)
		return (handle_two_commands(argv, msh));
	else
		return (handle_multiple_commands(argv, msh));
}

static int	exit_msh(char **argv, t_msh *msh, t_tkn *tokens)
{
	printf("exit\n");
	(void)argv;
	free_array(msh->envp, 0);
	free_array(msh->ex_envp, 0);
	free_msh(msh->cmds, msh, tokens);
	exit(msh->exit_status);
}

int	msh_exit(char **argv, t_msh *msh, t_tkn *tokens)
{
	if (msh->cmd_count >= 2)
		msh->exit_status = verify_arg(argv, msh, tokens);
	if (msh->flag_exit == false)
		msh->exit_status = exit_msh(argv, msh, tokens);
	return (msh->exit_status);
}
