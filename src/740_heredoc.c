/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   740_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 08:11:30 by luigi             #+#    #+#             */
/*   Updated: 2025/01/07 17:34:39 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	handle_heredoc_signal(int signal)
{
	if (signal == SIGINT)
	{
		ft_putstr_fd("\n", 1);
		g_signal = SIGINT;
		close(STDIN_FILENO);
	}
}

void	set_signals_to_here_doc(void)
{
	signal(SIGINT, handle_heredoc_signal);
	signal(SIGQUIT, SIG_DFL);
}

int	fill_fd_heredoc(int temp_fd, char *eof)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			free(line);
			if (g_signal == SIGINT)
				return (2);
			else
				return (1);
		}
		if (ft_strcmp(line, eof) == 0)
		{
			free(line);
			return (0);
		}
		write(temp_fd, line, ft_strlen(line));
		write(temp_fd, "\n", 1);
		free(line);
	}
	return (0);
}

void	start_heredoc(t_msh *msh, t_tkn *tokens)
{
	t_tkn		*tkn;
	t_heredoc	*hd;

	tkn = tokens;
	hd = msh->heredoc;
	while (tkn)
	{
		if (tkn->next && tkn->type == TKN_HEREDOC)
		{
			hd = msh->heredoc;
			if (hd->i == 0)
			{
				hd->i = 1;
				hd->eof = find_eof(tokens);
			}
			else
				assign_heredoc(&hd, tkn->next->name);
		}
		tkn = tkn->next;
	}
}

void	heredoc_exec(t_msh *msh, t_tkn *tokens)
{
	pid_t		pid;
	char		*i;
	int			status;
	int			len;
	t_heredoc	*heredoc;

	start_heredoc(msh, tokens);
	heredoc = msh->heredoc;
	msh->heredoc->len = heredoc_len(heredoc);
	len = heredoc_len(heredoc);
	while (heredoc && len--)
	{
		i = ft_itoa(heredoc->i);
		heredoc->fd_heredoc_path = ft_strjoin("/tmp/tmp_heredoc", i);
		free(i);
		pid = fork();
		if (pid == 0)
			heredoc_child_process(heredoc->eof, heredoc->fd_heredoc_path,
				msh, tokens);
		waitpid(pid, &status, 0);
		transform(tokens, heredoc->fd_heredoc_path);
		heredoc = heredoc->next;
	}
}
