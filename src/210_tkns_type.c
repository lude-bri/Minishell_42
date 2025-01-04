/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   210_tkns_type.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 10:50:15 by luigi             #+#    #+#             */
/*   Updated: 2024/12/22 15:19:32 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	token_len(t_tkn *tokens)
{
	int		i;

	i = 0;
	while (tokens)
	{
		i++;
		tokens = tokens->next;
	}
	return (i);
}

//create nodes for linked list -> token
t_tkn	*tkn_new(t_msh *msh, char *content)
{
	t_tkn	*node;

	node = malloc(sizeof(t_tkn));
	if (!node)
		return (0);
	node->name = ft_strdup(content);
	node->type = 0;
	node->cmdargs = NULL;
	node->next = NULL;
	node->left = NULL;
	node->right = NULL;
	node->cmd_type = 0;
	vector_push(&msh->tokens, node);
	return (node);
}

//verify and assign token type commands
void	verify_tkn_cmd(t_tkn *token)
{
	if (token->name && ft_strcmp(token->name, "exit") == 0)
		token->cmd_type = CMD_EXIT;
	else if (token->name && ft_strcmp(token->name, "pwd") == 0)
		token->cmd_type = CMD_PWD;
	else if (token->name && ft_strcmp(token->name, "echo") == 0)
		token->cmd_type = CMD_ECHO;
	else if (token->name && ft_strcmp(token->name, "cd") == 0)
		token->cmd_type = CMD_CD;
	else if (token->name && ft_strcmp(token->name, "export") == 0)
		token->cmd_type = CMD_EXPORT;
	else if (token->name && ft_strcmp(token->name, "env") == 0)
		token->cmd_type = CMD_ENV;
	else if (token->name && ft_strcmp(token->name, "unset") == 0)
		token->cmd_type = CMD_UNSET;
	else
		token->cmd_type = CMD_EXEC;
}

void	assign_tkn(t_tkn *token, char **av, t_msh *msh)
{
	handle_blank_and_null(token);
	if (token->type == TKN_BLANK || token->type == TKN_NULL)
		return ;
	handle_other_tokens(token, av, msh);
}

//create tokens
t_tkn	*tokenizer(t_msh *msh, char **av)
{
	int		i;
	char	**find;
	t_tkn	*new_token;
	t_tkn	*token;
	t_tkn	*current;

	i = -1;
	token = NULL;
	current = NULL;
	find = av;
	while (av[++i])
	{
		new_token = tkn_new(msh, av[i]);
		assign_tkn(new_token, find++, msh);
		if (new_token->type == TKN_CMD)
			verify_tkn_cmd(new_token);
		if (!token)
			token = new_token;
		else
			current->next = new_token;
		current = new_token;
	}
	token->len = token_len(token);
	return (token);
}
