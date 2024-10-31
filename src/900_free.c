/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   900_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 12:53:23 by luigi             #+#    #+#             */
/*   Updated: 2024/10/31 12:31:29 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	free_array(char **str, int error)
{
	int		i;

	if (!str)
		return (error);
	i = -1;
	while (str[++i])
		free(str[i]);
	free(str);
	return (error);
}

void	free_arg(char **argv)
{
	int	i;

	i = 0;
	if (!argv)
		return ;
	while (argv[i])
	{
		free(argv[i]);
		argv[i] = NULL;
		i++;
	}
	free(argv);
	argv = NULL;
}

int	free_cmds(t_command *command, t_msh *msh)
{
	char	**av;
	char	*cmd;

	av = command->av;
	cmd = command->cmd;
	msh->cmd_count = 0;
	return (0);
}

void	free_tokens(t_tkn *token)
{
	t_tkn	*current;

	while (token)
	{
		current = token;
		token = token->next;
		free(current->name);
		free(current);
	}
}
