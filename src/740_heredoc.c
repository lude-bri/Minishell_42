/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   740_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 08:11:30 by luigi             #+#    #+#             */
/*   Updated: 2024/11/18 08:14:37 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	heredoc(t_tkn *tokens, t_msh *msh)
{
	int		pipe_fd[2];
	char	*line;

	if (pipe(pipe_fd) < 0)
	{
		perror("pipe");
		free_msh(msh->cmds, msh, tokens);
		exit(EXIT_FAILURE);
	}
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, tokens->next->name) == 0)
			break ;
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	free(line);
	close(pipe_fd[1]);
	dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
}
