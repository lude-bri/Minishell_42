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

t_tkn	*to_parse(t_msh *msh)
{
	t_tkn	*tokens;
	char	*line;

	line = build_prompt(msh);
	if (!line)
		return (NULL);
	add_history(line);
	msh->cmds = ft_calloc(1, sizeof(t_command));
	if (!msh->cmds)
		return (NULL);
	msh->cmds->av = split_input(line, msh); //necessario revisar!!
	if (!msh->cmds->av || !*msh->cmds->av)
	{
		free_arg(msh->cmds->av);
		free(msh->cmds);
		return (NULL);
	}
	msh->cmd_count = ft_matrixlen(msh->cmds->av);
	tokens = tokenizer(msh, msh->cmds->av);
	if (syntax_check(msh, tokens) == FAILURE) //meter no inicio de tudo
	{
		free_arg(msh->cmds->av);
		free(msh->cmds);
		return (NULL);
	}
	msh->pipe_count = count_pipes(tokens);
	msh->line = ft_strdup(line); //ter o line para fazer verificacao no echo
	if (line)
	{
		free(line);
		line = NULL;
	}
	return (tokens);
}
