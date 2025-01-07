/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   900_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 12:53:23 by luigi             #+#    #+#             */
/*   Updated: 2025/01/06 20:09:22 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_heredoc(t_heredoc *heredoc)
{
	t_heredoc	*tmp;

	while (heredoc)
	{
		tmp = heredoc->next;
		if (heredoc->fd_heredoc_path)
		{
			free(heredoc->fd_heredoc_path);
			heredoc->fd_heredoc_path = NULL;
		}
		free(heredoc);
		heredoc = NULL;
		heredoc = tmp;
	}
}

void	free_vector(t_vector *vector)
{
	int		i;
	t_tkn	*token;

	i = 0;
	while (i < vector->count)
	{
		token = vector->buffer[i];
		if (token != NULL)
		{
			free(token->name);
			free(token);
			token = NULL;
		}
		i++;
	}
	if (vector->buffer != NULL)
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
	if (msh->line)
		free(msh->line);
	if (token)
		free_vector(&msh->tokens);
	if (command)
		free(command);
	if (msh->heredoc)
	{
		free_heredoc(msh->heredoc);
		msh->heredoc = NULL;
	}
}
