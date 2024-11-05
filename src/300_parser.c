/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   300_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:28:36 by luigi             #+#    #+#             */
/*   Updated: 2024/10/31 17:29:48 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	{
		//free_cmds(msh->cmds, 0);
		return (NULL);
	}
	msh->cmd_count = ft_matrixlen(msh->cmds->av);
	tokens = tokenizer(msh->cmds->av);
	if (line)
	{
		free(line);
		line = NULL;
	}
	return (tokens);
}
