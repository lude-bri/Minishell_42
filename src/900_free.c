/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   900_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 12:53:23 by luigi             #+#    #+#             */
/*   Updated: 2024/11/09 14:02:40 by luigi            ###   ########.fr       */
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

void	free_msh(t_command *command, t_msh *msh, t_tkn *token)
{
	if (msh->cmds->av)
		free_arg(msh->cmds->av);
	if (msh->envp)
		free_arg(msh->envp);
	if (token)
		free_tokens(token);
	if (command)
		free(command);
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
