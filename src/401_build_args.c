/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   401_build_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 12:18:32 by luigi             #+#    #+#             */
/*   Updated: 2024/12/23 00:11:09 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

//count the len of arguments WITHOUT pipes
static int	count_tokens(t_tkn *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		if (tokens->type == TKN_PIPE)
			break ;
		else
		{
			count++;
			tokens = tokens->next;
		}
	}
	return (count);
}

void	free_args(char **args, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(args[i]);
		i++;
	}
	free(args);
}

char	**allocate_and_copy_args(t_tkn *tokens, int num_tokens)
{
	char	**args;
	int		i;

	args = (char **)malloc(sizeof(char *) * (num_tokens + 1));
	i = 0;
	if (!args)
		return (NULL);
	while (tokens)
	{
		if (tokens->type == TKN_PIPE || tokens->type == TKN_IN
			|| tokens->type == TKN_OUT || tokens->type == TKN_APPEND
			|| tokens->type == TKN_HEREDOC)
			break ;
		args[i] = ft_strdup(tokens->name);
		if (!args[i])
			return (free_args(args, i), NULL);
		i++;
		tokens = tokens->next;
	}
	args[i] = NULL;
	return (args);
}

char	**build_args(t_tkn *tokens)
{
	int	num_tokens;

	num_tokens = count_tokens(tokens);
	return (allocate_and_copy_args(tokens, num_tokens));
}
