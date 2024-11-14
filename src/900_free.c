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

void	free_vector(t_vector *vector)
{
	int		i;
	t_tkn	*token;

	i = 0;
	while (i < vector->count)
	{
		token = vector->buffer[i];
		free(token->name);
		free(token);
		i++;
	}
	free(vector->buffer);
	vector->buffer = NULL;
	vector->count = 0;
	vector->size = 0;
}

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
	if (token)
		free_vector(&msh->tokens);
	if (command)
		free(command);
}
