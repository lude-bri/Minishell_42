/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   901_free_heredocs.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 17:18:16 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/07 17:35:19 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_hd(t_heredoc *heredoc)
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
		heredoc = tmp;
	}
}

void	free_vector_2(t_vector *vector, t_msh *msh)
{
	int		i;
	t_tkn	*token;

	(void)msh;
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

void	free_all_heredoc(t_msh *msh)
{
	if (msh->envp)
		free_arg(msh->envp);
	if (msh->ex_envp)
		free_arg(msh->ex_envp);
	if (msh->cmds)
	{
		if (msh->cmds->av)
			free_arg(msh->cmds->av);
		free(msh->cmds);
	}
	if (msh->line)
		free(msh->line);
	free_vector_2(&msh->tokens, msh);
	if (msh->heredoc->fd_heredoc_path)
		free_hd(msh->heredoc);
}
