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

/**
 * @brief Calculates the length of a linked list of tokens.
 *
 * Iterates through the token list and counts how many nodes it contains.
 *
 * @param tokens Pointer to the head of the token list.
 * @return The number of tokens in the list.
 */
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

/**
 * @brief Creates a new token node with the given content and default values.
 *
 * Allocates memory for a new `t_tkn` struct, initializes all fields,
 * and adds it to the token vector in `msh`.
 *
 * @param msh Pointer to the shell state structure.
 * @param content The string content of the token (e.g., a word or command).
 * @return Pointer to the newly created token, or NULL on allocation failure.
 */
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

/**
 * @brief Assigns a command type to a token based on its content.
 *
 * Compares the token's name to known built-in commands and sets its `cmd_type`
 * accordingly. Defaults to `CMD_EXEC` if no match is found.
 *
 * @param token Pointer to the token to analyze and classify.
 */
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

/**
 * @brief Assigns a type and additional properties to a token.
 *
 * Handles blank/null detection and passes the token to further
 * processing logic if valid.
 *
 * @param token Pointer to the token to analyze.
 * @param av Current position in the argument vector (used in token handling).
 * @param msh Pointer to the shell state.
 */
void	assign_tkn(t_tkn *token, char **av, t_msh *msh)
{
	handle_blank_and_null(token);
	if (token->type == TKN_BLANK || token->type == TKN_NULL)
		return ;
	handle_other_tokens(token, av, msh);
}

/**
 * @brief Converts a list of input arguments into a linked list of token nodes.
 *
 * For each word in `av`, creates a token, assigns its type, and detects if it is a command.
 * Returns the head of the linked list of tokens.
 *
 * @param msh Pointer to the shell state (used for vector storage and context).
 * @param av NULL-terminated array of strings (already parsed arguments).
 * @return Pointer to the head of the newly constructed token list.
 */
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
