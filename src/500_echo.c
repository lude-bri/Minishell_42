/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   500_echo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:48:34 by luigi             #+#    #+#             */
/*   Updated: 2024/11/18 09:50:02 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// static int	check_echo_syntax(t_msh *msh, char *argv)
// {
// 	if (ft_strcmp(argv, "<") == 0 || ft_strcmp(argv, ">") == 0)
// 	{
// 		if (syntax_check_redirs(msh, NULL) != SUCCESS)
// 			return (FAILURE);
// 	}
// 	else if (ft_strcmp(argv, "|") == 0)
// 	{
// 		printf("msh: syntax error near unexpected token `|'\n");
// 		msh->exit_status = 2;
// 		return (FAILURE);
// 	}
// 	return (SUCCESS);
// }

// int	msh_echo(t_msh *msh, t_tkn *tokens)
// {
// 	int	i;
// 	int	newline;
//
// 	(void)msh;
// 	if (!tokens->cmdargs[1])
// 	{
// 		ft_printf("\n");
// 		return (0);
// 	}
// 	else if (tokens->cmdargs[2] != NULL)
// 		if (check_echo_syntax(msh, tokens->cmdargs[2]) != SUCCESS)
// 			return (FAILURE);
// 	i = 1;
// 	newline = 1;
// 	if (tokens->cmdargs[1] && ft_strncmp(tokens->cmdargs[1], "-n", 2) == 0)
// 	{
// 		newline = 0;
// 		i = 2;
// 	}
// 	while (tokens->cmdargs[i])
// 	{
// 		ft_printf("%s", tokens->cmdargs[i]);
// 		if (tokens->cmdargs[i + 1])
// 			ft_printf(" ");
// 		i++;
// 	}
// 	if (newline)
// 		ft_printf("\n");
// 	return (0);
// }

static int	is_delimiter(char *argv)
{
	if (ft_strncmp(argv, "<", 1) == 0)
		return (SUCCESS);
	else if (ft_strncmp(argv, ">", 1) == 0)
		return (SUCCESS);
	else if (ft_strncmp(argv, "|", 1) == 0)
		return (SUCCESS);
	else if (ft_strncmp(argv, ">>", 1) == 0)
		return (SUCCESS);
	return (FAILURE);
}


int	msh_echo(char **argv)
{
	int	i;
	int	newline;

	if (!argv[1])
	{
		ft_printf("\n");
		return (0);
	}
	i = 1;
	newline = 1;
	if (argv[1] && ft_strncmp(argv[1], "-n", 2) == 0)
	{
		newline = 0;
		i = 2;
	}
	while (argv[i])
	{
		if (is_delimiter(argv[i]) == SUCCESS)
			break ;
		ft_printf("%s", argv[i]);
		if (argv[i + 1])
			ft_printf(" ");
		i++;
	}
	if (newline)
		ft_printf("\n");
	return (0);
}
