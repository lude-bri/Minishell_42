/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   211_tkns_type_handler.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/22 15:01:31 by mde-agui          #+#    #+#             */
/*   Updated: 2024/12/24 12:39:31 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	verify_quotes(char operator, char *line)
{
	int		i;

	i = 0;
	while (line[i] != operator)
		i++;
	if (i > 0)
	{
		if (line[i + 1] == '"' || line[i - 1] == '"'
			|| line[i + 1] == '\'' || line[i - 1] == '\'')
			return (SUCCESS);
	}
	return (FAILURE);
}

int	verify_inbetween_quotes(char operator, char *line)
{
	int		i;

	i = 0;
	while (line[i] != operator)
		i++;
	if (i > 0)
	{
		if ((line[i + 1] == '"' && line[i - 1] == '"')
			|| (line[i + 1] == '\'' && line[i - 1] == '\''))
			return (SUCCESS);
	}
	return (FAILURE);
}

void	handle_heredoc_token(t_tkn *token, char *line)
{
	if (verify_quotes(token->name[0], line) == FAILURE)
		token->type = TKN_HEREDOC;
	else
		token->type = TKN_CMD;
}

void	handle_other_tokens(t_tkn *tkn, char **av, t_msh *msh)
{
	char	*line;

	line = msh->line;
	if (tkn->name[0] == '|')
		handle_pipe_token(tkn, line);
	else if (*av[0] == '>' && av[1] != NULL)
	{
		if (ft_strncmp(*av, ">>", 2) == 0)
			handle_append_token(tkn, line);
		else
			handle_out_token(tkn, line);
	}
	else if (tkn->name[0] == '<' && tkn->name[1] == '<' && tkn->name[2] != '\"')
		handle_heredoc_token(tkn, line);
	else if (tkn->name[0] == '<' && av[1] != NULL)
		handle_in_token(tkn, line);
	else if (tkn->name[0] == '>')
	{
		if (verify_quotes(tkn->name[0], line) == FAILURE)
			handle_out_token(tkn, line);
		else
			tkn->type = TKN_CMD;
	}
	else
		tkn->type = TKN_CMD;
}
