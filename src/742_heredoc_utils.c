/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   742_heredoc_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/02 15:58:39 by luigi             #+#    #+#             */
/*   Updated: 2025/01/07 17:23:15 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	transform(t_tkn *tokens, char *path)
{
	t_tkn	*tkn;

	tkn = tokens;
	while (tkn->type != TKN_HEREDOC)
		tkn = tkn->next;
	tkn->type = TKN_IN;
	tkn = tkn->next;
	if (tkn)
	{
		tkn->type = TKN_REDIR_ARG;
		free(tkn->name);
		tkn->name = ft_strdup(path);
	}
}

void	assign_heredoc(t_heredoc **heredoc, char *eof)
{
	t_heredoc	*tmp_hd;

	tmp_hd = *heredoc;
	while (tmp_hd->next)
		tmp_hd = tmp_hd->next;
	tmp_hd->next = ft_calloc(1, sizeof(t_heredoc));
	tmp_hd->next->i = tmp_hd->i + 1;
	tmp_hd->next->count_hd = tmp_hd->count_hd;
	tmp_hd->next->eof = eof;
	tmp_hd->next->next = NULL;
}

char	*find_eof(t_tkn *tokens)
{
	t_tkn	*tmp;

	tmp = tokens;
	while (tmp->type != TKN_HEREDOC)
		tmp = tmp->next;
	if (tmp->next)
		return (tmp->next->name);
	return (NULL);
}

int	heredoc_len(t_heredoc *heredoc)
{
	int		i;

	i = 0;
	while (heredoc)
	{
		i++;
		heredoc = heredoc->next;
	}
	return (i);
}

int	find_heredoc(t_tkn *tokens)
{
	t_tkn	*tkn;

	tkn = tokens;
	while (tkn)
	{
		if (tkn->type == TKN_HEREDOC)
			return (SUCCESS);
		tkn = tkn->next;
	}
	return (FAILURE);
}
