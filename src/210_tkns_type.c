/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   210_tkns_type.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:50:15 by luigi             #+#    #+#             */
/*   Updated: 2024/10/31 12:42:02 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static t_tkn	*tkn_new(char *content);
static void		assign_tkn(t_tkn *token);
static void		verify_tkn_cmd(t_tkn *token);
static void		verify_tkn(t_tkn *token);

t_tkn	*tokenizer(char **av)
{
	int		i;
	t_tkn	*new_token;
	t_tkn	*token;
	t_tkn	*current;

	i = -1;
	token = NULL;
	current = NULL;
	while (av[++i])
	{
		new_token = tkn_new(av[i]);
		assign_tkn(new_token);
		if (new_token->type == TKN_CMD)
			verify_tkn_cmd(new_token); //classificar o comando especifico token
		else
			verify_tkn(new_token);//verificar se nao ha erros associados ao tipo de token
		if (!token)
			token = new_token;
		else
			current->next = new_token;
		current = new_token;
	}
	return (token);
}

static t_tkn	*tkn_new(char *content)
{
	t_tkn	*node;

	node = ft_calloc(1, sizeof(t_tkn));
	if (!node)
		return (0);
	node->name = ft_strdup(content);
	node->type = 0;
	node->next = NULL;
	return (node);
}

static void	assign_tkn(t_tkn *token)
{
	if (token->name[0] == ' ' || token->name[0] == '\n' || token->name[0] == '\v' 
			|| token->name[0] == '\t' || token->name[0] == '\r'
			|| token->name[0] == '\f')
		token->type = TKN_BLANK;
	else if (token->name[0] == '\0')
		token->type = TKN_NULL;
	else if (token->name[0] == '|')
		token->type = TKN_PIPE;
	else if (token->name[0] == '>')
		token->type = TKN_OUT;
	else if (token->name[0] == '>' && token->name[1] == '>')
		token->type = TKN_OUT;
	else if (token->name[0] == '<')
		token->type = TKN_IN;
	else if (token->name[0] == '<' && token->name[1] == '<')
		token->type = TKN_HEREDOC;
	else
		token->type = TKN_CMD;
}

static void	verify_tkn_cmd(t_tkn *token)
{
	(void)token;
}

static void	verify_tkn(t_tkn *token)
{
	(void)token;
}


/*
typedef struct s_tkn 
{
	t_token_group	type;
	char			*name;
	int				len;
	struct s_tkn    *next;
}		t_tkn;

*/
