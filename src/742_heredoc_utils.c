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

/**
 * @brief Replaces the heredoc token with an input redirection and updates its path.
 *
 * Converts the token type from `TKN_HEREDOC` to `TKN_IN` and replaces the next
 * token's name with the path to the temporary heredoc file.
 *
 * @param tokens Token list where the heredoc was found.
 * @param path Path to the heredoc temporary file.
 */
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

/**
 * @brief Appends a new heredoc node to the heredoc linked list.
 *
 * Increments the heredoc index and assigns the EOF delimiter.
 *
 * @param heredoc Pointer to the current heredoc list.
 * @param eof The EOF string used to terminate heredoc input.
 */
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

/**
 * @brief Searches for the heredoc token and retrieves its associated EOF string.
 *
 * @param tokens Token list to search through.
 * @return char* The EOF delimiter, or NULL if not found.
 */
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

/**
 * @brief Counts how many heredoc structures exist in the linked list.
 *
 * @param heredoc Pointer to the head of the heredoc list.
 * @return int Number of heredoc nodes.
 */
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

/**
 * @brief Checks whether the token list contains a heredoc token.
 *
 * @param tokens List of tokens to inspect.
 * @return int Returns SUCCESS (1) if a heredoc is found, otherwise FAILURE (0).
 */
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
