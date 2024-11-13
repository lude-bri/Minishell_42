/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   900_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 12:53:23 by luigi             #+#    #+#             */
/*   Updated: 2024/11/09 14:02:40 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

extern void	free_tkns(void);

// static int	token_len(t_tkn *tokens)
// {
// 	int	i;
//
// 	i = 0;
// 	while (tokens)
// 	{
// 		i++;
// 		tokens = tokens->next;
// 	}
// 	return (i);
// }
//
int	free_array(char **str, int error)
{
	int		i;

	// perror(__func__);
	if (!str)
		return (error);
	i = -1;
	while (str[++i])
		free(str[i]);
	free(str);
	return (error);
}

void	free_arg(char **argv)
{
	int	i;

	i = 0;
	
	// perror(__func__);
	if (!argv)
		return ;
	while (argv[i])
	{
		free(argv[i]);
		argv[i] = NULL;
		i++;
	}
	free(argv);
	argv = NULL;
}

void	free_msh(t_command *command, t_msh *msh, t_tkn *token)
{
	// perror(__func__);
	if (msh->cmds->av)
		free_arg(msh->cmds->av);
	if (token)
		free_vector(&msh->tokens);
		// free_tokens(token);
		// free_tkns();
	if (command)
		free(command);
}

void	free_tokens(t_tkn *token)
{
	// int		i;
	// int		len;
	// t_tkn	*tmp;
	//
	// i = 0;
	// len = token_len(token);
	// while (len >= i)
	// {
	// 	tmp = token->next;
	// 	free(token->name);
	// 	free(token);
	// 	token = tmp;
	// 	i++;
	// }

	t_tkn	*tmp;
	int		counter = 0;

	while (token)
	{
		tmp = token->next;
		free(token->name);
		free(token);
		counter += 1;
		token = tmp;
	}
	printf("%i\n", counter);
}
