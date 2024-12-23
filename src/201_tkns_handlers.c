/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   201_tkns_handlers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 23:11:56 by mde-agui          #+#    #+#             */
/*   Updated: 2024/12/22 00:10:30 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	handle_input_redir(const char *input, t_tkn_op *sp, t_msh *msh)
{
	if (input[sp->i] == '<' && input[sp->i + 1] == '<')
		handle_double_in_redir(input, sp);
	else if (input[sp->i] == '<')
		handle_single_in_redir(input, sp, msh);
}

void	handle_output_redir(const char *input, t_tkn_op *sp, t_msh *msh)
{
	if (input[sp->i] == '>')
	{
		if (msh->len == 1)
			return ;
		else if (msh->len == 2)
		{
			if (sp->i > 0 && input[sp->i - 1] == '\0')
			{
				if (input[sp->i + 1] == '"')
					sp->argv[(sp->j)++] = ft_strdup("\">\"");
				else
					return ;
			}
		}
		else
			sp->argv[(sp->j)++] = ft_strdup(">");
		(sp->i)++;
	}
}

void	handle_pipes(const char *input, t_tkn_op *sp)
{
	if (sp->i > 0 && (input[sp->i - 1] == '"' || input[sp->i + 1] == '"'))
		sp->argv[(sp->j)++] = ft_strdup("\"|\"");
	else
		sp->argv[(sp->j)++] = ft_strdup("|");
	(sp->i)++;
}

void	handle_expand(const char *input, t_tkn_op *sp, t_msh *msh)
{
	char	*expanded;

	if (input[sp->i + 1] == '>' || input[sp->i + 1] == '<')
	{
		perror("msh: syntax error near unexpected token `newline'\n");
		return ;
	}
	if (input[sp->i + 1] == '\0' || input[sp->i + 1] == '\"')
	{
		sp->argv[(sp->j)++] = ft_strdup("$");
		(sp->i)++;
	}
	else
	{
		expanded = expand_var(input, &sp->i, msh);
		if (expanded)
		{
			sp->argv[(sp->j)++] = ft_strdup(expanded);
			free(expanded);
		}
	}
}

void	handle_quotes(const char *input, t_tkn_op *sp, t_msh *msh)
{
	if (input[sp->i] == '\'')
		sp->argv[sp->j++] = handle_single_quotes(input, &sp->i);
	else if (input[sp->i] == '"')
	{
		sp->start = ++(sp->i);
		if (input[sp->i] == '"')
			sp->argv[sp->j++] = ft_strdup("");
		else
			sp->argv[sp->j++] = handle_double_quotes(input, &sp->i, msh);
	}
}
