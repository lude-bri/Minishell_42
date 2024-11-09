/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   401_build_args.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 12:18:32 by luigi             #+#    #+#             */
/*   Updated: 2024/11/09 12:51:03 by luigi            ###   ########.fr       */
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

//transform linked list into char ** to execute in execve
char	**build_args(t_tkn *tokens)
{
	int		i;
	int		j;
	int		num_tokens;
	char	**args;

	num_tokens = count_tokens(tokens);
	args = (char **)malloc(sizeof(char *) * (num_tokens + 1));
	if (!args)
		return (NULL);
	i = 0;
	while (tokens)
	{
		if (tokens->type == TKN_PIPE)
			break ;
		args[i] = ft_strdup(tokens->name);
		if (!args[i])
		{
			j = 0;
			while (j < i)
			{
				free(args[j]);
				j++;
			}
			free(args);
			return (NULL);
		}
		i++;
		tokens = tokens->next;
	}
	args[i] = NULL;
	return (args);
}
