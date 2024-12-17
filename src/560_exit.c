/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   560_exit.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:48:51 by luigi             #+#    #+#             */
/*   Updated: 2024/12/14 18:37:05 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


static int	exit_msh(char **argv, t_msh *msh, t_tkn *tokens);
static int	verify_arg(char **argv, t_msh *msh, t_tkn *tokens);

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
		if (ft_isalpha(str[i]))
			return (1);
		i++;
	}
	return (0);
}

int	msh_exit(char **argv, t_msh *msh, t_tkn *tokens)
{
	if (msh->cmd_count >= 2)
		msh->exit_status = verify_arg(argv, msh, tokens);
	msh->exit_status = exit_msh(argv, msh, tokens);
	return (msh->exit_status);
}

static int exit_msh(char **argv, t_msh *msh, t_tkn *tokens)
{
    printf("exit\n");

	(void)argv;
    // if (argv != NULL)
    // {
    //     free_arg(argv);      // Free argv
    //     argv = NULL;         // Prevent dangling pointer
    // }
    // if (msh->cmds && msh->cmds->av)
    // {
    //     free_arg(msh->cmds->av);  // Free msh->cmds->av separately
    //     msh->cmds->av = NULL;
    // }
    free_array(msh->envp, 0); // Free envp
    free_msh(msh->cmds, msh, tokens);
    
    exit(msh->exit_status);
}



// static int	exit_msh(char **argv, t_msh *msh, t_tkn *tokens)
// {	
// 	printf("exit\n");
// 	free_arg(argv);
// 	free_array(msh->envp, 0);
// 	free_msh(msh->cmds, msh, tokens);
// 	// free_arg(msh->cmds->av);
// 	// free(msh->cmds);
// 	// free(msh->line);
// 	// free_vector(&msh->tokens);
// 	exit(msh->exit_status);
// }

static int	is_sign(char *argv)
{
	return ((ft_strncmp(argv, "+", 1) == 0) || (ft_strncmp(argv, "-", 1) == 0));
}

static int	verify_arg(char **argv, t_msh *msh, t_tkn *tokens)
{
	if (!argv[1])
		msh->exit_status = exit_msh(argv, msh, tokens);	
	if (msh->cmd_count == 2)
	{
		if (is_letter(argv[1]))
		{
			ft_printf("msh: exit: %s: numeric argument required\n", argv[1]);
			return (2);
		}
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
	}
	else
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
			ft_printf("msh: exit: %s: numeric argument required\n", argv[1]);
			return (2);
		}
		else 
		{
			ft_printf("msh: exit: too many arguments\n");
			return (1);
		}
	}
	return (msh->exit_status);
}



























// int	msh_exit(char **argv, t_msh *msh)
// {
// 	if (msh->cmd_count > 2)
// 	{
// 		if (is_letter(argv[1]) || msh->cmd_count >= 3)
// 	{
// 			ft_printf("msh: exit: %s: numeric argument required\n", argv[1]);
// 			msh->exit_status = 2;
// 		}
// 		else
// 		{
// 			ft_printf("msh: exit: too many arguments\n");
// 			msh->exit_status = 1;
// 			return (msh->exit_status);
// 		}
// 	}
// 	if (!argv[1])
// 	{
// 		ft_printf("exit\n");
// 		free_arg(argv);
// 		free_array(msh->envp, 0);
// 		free(msh->cmds);
// 		free(msh->line);
// 	free_vector(&msh->tokens);
// 		exit(msh->exit_status);
// 	}
// 	if (is_letter(argv[1]))
// 	{
// 		ft_printf("msh: exit: %s: numeric argument required\n", argv[1]);
// 		msh->exit_status = 2;
// 	}
// 	if ((ft_strncmp(argv[1], "+", 1) == 0) || ft_strncmp(argv[1], "-", 1) == 0)
// 	{	
// 		if (is_num(argv[2]) && ft_strncmp(argv[1], "+", 1) == 0)
// 			msh->exit_status = ft_atoi(argv[2]);
// 		else
// 			msh->exit_status = 256 - ft_atoi(argv[2]);
// 	}
// 	else if (msh->exit_status != 2)
// 	{
// 		if (is_letter(argv[2]))
// 		{
// 			ft_printf("msh: exit: too many arguments\n");
// 			msh->exit_status = 1;
// 			return (msh->exit_status);
// 		}
// 		else
// 		{
// 			ft_printf("msh: exit: %s: numeric argument required\n", argv[1]);
// 			msh->exit_status = 2;
// 		}
// 	}
// 	if (is_num(argv[1]))
// 		msh->exit_status = ft_atoi(argv[1]);
// 	ft_printf("exit\n");
// 	free_arg(argv);
// 	free_array(msh->envp, 0);
// 	free(msh->cmds);
// 	free(msh->line);
// 	free_vector(&msh->tokens);
// 	exit(msh->exit_status);
// }
