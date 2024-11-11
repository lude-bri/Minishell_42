/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   300_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:28:36 by luigi             #+#    #+#             */
/*   Updated: 2024/11/11 11:43:50 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	count_pipes(t_tkn *tokens)
{
	int		counter;
	
	counter = 0;
	while (tokens)
	{
		if (!counter)
			counter = 1;
		if (tokens->type == TKN_PIPE)
			counter++;
		tokens = tokens->next;
	}
	return (counter);
}

t_tkn	*to_parse(t_msh *msh, char *line)
{
	t_tkn	*tokens;

	if (!line)
		return (NULL);
	add_history(line);
	msh->cmds = ft_calloc(1, sizeof(t_command));
	if (!msh->cmds)
		return (NULL);
	msh->cmds->av = split_input(line, msh);
	if (!msh->cmds->av)
		return (NULL);
	msh->cmd_count = ft_matrixlen(msh->cmds->av);
	tokens = tokenizer(msh->cmds->av);
	if (syntax_check(msh, tokens) == FAILURE)
		return (NULL);
	msh->pipe_count = count_pipes(tokens);
	if (line)
	{
		free(line);
		line = NULL;
	}
	return (tokens);
}
