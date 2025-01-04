/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   900_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 12:53:23 by luigi             #+#    #+#             */
/*   Updated: 2024/11/14 10:35:59 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	free_eof(char *eof)
{
	if (eof)
	{
		free(eof);
		eof = NULL;
	}
}

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
		heredoc->i = 0;
		free(heredoc->next);
		heredoc->next = NULL;
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
			token->name = NULL;
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

// static void free_hd(t_heredoc *heredoc)
// {
//     t_heredoc *tmp;
//
//     while (heredoc)
//     {
//         tmp = heredoc->next;
//
//         // Liberar campos e o nó atual
//         if (heredoc->eof)
//         {
// 			free(heredoc->eof);
// 			heredoc->eof = NULL;
// 		}
//         if (heredoc->fd_heredoc_path)
// 		{
// 			free(heredoc->fd_heredoc_path);
// 			heredoc->fd_heredoc_path = NULL;
// 		}
// 		// free(heredoc);
//         // Avançar para o próximo nó
//         heredoc = tmp;
//     }
// }

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
	// if (msh->heredoc.len >= 2)
		// free_hd(&msh->heredoc);
	if (msh->heredoc.fd_heredoc_path)
	{
		free_heredoc(&msh->heredoc);
		msh->heredoc.fd_heredoc_path = NULL;
	}
	if (msh->heredoc.eof)
	{
		free_eof(msh->heredoc.eof);
		msh->heredoc.eof = NULL;
	}
}
