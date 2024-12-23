/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   740_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 08:11:30 by luigi             #+#    #+#             */
/*   Updated: 2024/12/23 03:17:11 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Flag global para indicar interrupção do heredoc
static volatile int	g_heredoc_interrupted = 0;

static void	handle_signal(int sig)
{
	if (sig == SIGINT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		g_heredoc_interrupted = 1;
	}
}

void	setup_pipe_and_signals(t_tkn *tokens, t_msh *msh, int *pipe_fd)
{
	if (pipe(pipe_fd) < 0)
	{
		perror("pipe");
		free_msh(msh->cmds, msh, tokens);
		exit(EXIT_FAILURE);
	}
	signal(SIGINT, handle_signal);
}

void	read_n_write_lines(t_tkn *tokens, t_msh *msh, char *arg, int *pipe_fd)
{
	char	*line;

	line = NULL;
	while (1)
	{
		if (g_heredoc_interrupted)
		{
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			free_msh(msh->cmds, msh, tokens);
			exit(130);
		}
		line = readline("> ");
		if (!line || ft_strcmp(line, arg) == 0)
			break ;
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
	}
	free(line);
	close(pipe_fd[1]);
}

//same thing as above?
void	read_n_write_lines2(t_tkn *tokens, t_msh *msh, char *arg, int *pipe_fd)
{
	char	*line;

	line = NULL;
	while (1)
	{
		if (g_heredoc_interrupted)
		{
			close(pipe_fd[0]);
			close(pipe_fd[1]);
			free_msh(msh->cmds, msh, tokens);
			exit(130);
		}
		line = readline("> ");
		if (!line || ft_strcmp(line, arg) == 0)
			break ;
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
		line = NULL;
	}
	free(line);
	close(pipe_fd[1]);
}

void	heredoc(t_tkn *tokens, t_msh *msh, char *arg, int flag)
{
	int	pipe_fd[2];

	setup_pipe_and_signals(tokens, msh, pipe_fd);
	read_n_write_lines2(tokens, msh, arg, pipe_fd);
	if (flag == 0)
		dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	signal(SIGINT, SIG_DFL);
}
