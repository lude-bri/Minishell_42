/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   300_parser.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/29 10:28:36 by luigi             #+#    #+#             */
/*   Updated: 2024/12/22 23:26:37 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	count_pipes(t_tkn *tokens)
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

void	error_syntax(t_msh *msh)
{
	if (*msh->cmds->av[0] == '|')
		printf("msh: syntax error near unexpected token `|'\n");
	else
		printf("msh: syntax error near unexpected token `newline'\n");
	msh->exit_status = 2;
}

int	is_operator(char *line)
{
	if (ft_strcmp(line, "") == 0)
		return (FAILURE);
	if (*line == '\0')
		return (FAILURE);
	if (*line == '|' || *line == '<' || *line == '>')
		return (SUCCESS);
	return (FAILURE);
}

int	syntax(t_msh *msh, char *line)
{
	char	*str;

	str = *msh->cmds->av;
	if (initial_syntax_checks(msh, line, str) == FAILURE)
		return (FAILURE);
	if (detailed_syntax_checks(msh, str, line) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
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
	msh->cmds->av = split_input(line, msh);
	if (!msh->cmds->av || !*msh->cmds->av)
		return (free_arg(msh->cmds->av), free(msh->cmds), NULL);
	if (syntax(msh, line) == FAILURE)
		return (free_arg(msh->cmds->av), free(msh->cmds), NULL);
	msh->cmd_count = ft_matrixlen(msh->cmds->av);
	msh->line = ft_strdup(line);
	tokens = tokenizer(msh, msh->cmds->av);
	msh->pipe_count = count_pipes(tokens);
	if (line)
	{
		free(line);
		line = NULL;
	}
	return (tokens);
}
