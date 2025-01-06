/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   740_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 08:11:30 by luigi             #+#    #+#             */
/*   Updated: 2025/01/02 16:10:13 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Flag global para indicar interrupção do heredoc
static volatile int	g_heredoc_interrupted = 0;

static void	handle_heredoc_signal(int signal)
{
	if (signal == SIGINT)
	{
		ft_putstr_fd("\n", 1);
		g_heredoc_interrupted = SIGINT;
		close(STDIN_FILENO);
	}
}

static void	set_signals_to_here_doc(void)
{
	signal(SIGINT, handle_heredoc_signal);
	signal(SIGQUIT, SIG_DFL);
}

// static void	heredoc_child_process(char *eof, char *temp_path)
// {
// 	int		temp_fd;
// 	char	*line;
//
// 	temp_fd = open(temp_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (temp_fd < 0)
// 		exit(EXIT_FAILURE);
// 	set_signals_to_here_doc();
// 	while (1)
// 	{
// 		line = readline("> ");
// 		if (!line)
// 		{
// 			free(line);
// 			if (g_heredoc_interrupted == SIGINT)
// 				break ;
// 		}
// 		if (!line || ft_strcmp(line, eof) == 0)
// 			break ;
// 		write(temp_fd, line, strlen(line));
// 		write(temp_fd, "\n", 1);
// 		free(line);
// 	}
// 	free(line);
// 	close(temp_fd);
// 	exit (EXIT_SUCCESS);
// }


static int	fill_fd_heredoc(int temp_fd, char *eof)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			free(line);
			if (g_heredoc_interrupted == SIGINT)
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

static void free_hd(t_heredoc *heredoc)
{
    t_heredoc *tmp;

    while (heredoc)
    {
        tmp = heredoc->next;

        // Liberar campos e o nó atual
        if (heredoc->eof)
        {
			free(heredoc->eof);
			heredoc->eof = NULL;
		}
        if (heredoc->fd_heredoc_path)
		{
			free(heredoc->fd_heredoc_path);
			heredoc->fd_heredoc_path = NULL;
		}
		// free(heredoc->next);
        // Avançar para o próximo nó
        heredoc = tmp;
    }
}

static void	free_vector_2(t_vector *vector, t_msh *msh)
{
	int		i;
	t_tkn	*token;

	i = 0;
	while (i < vector->count)
	{
		token = vector->buffer[i];
		if (token != NULL)
		{
			free(token->name);
			token->name = NULL;
			msh->heredoc.eof = NULL;
			free(token);
			token = NULL;
		}
		i++;
	}
	if (vector->buffer != NULL)
		free(vector->buffer);
	vector->buffer = NULL;
	vector->count = 0;
	vector->size = 0;
}

static void	free_all_heredoc(t_msh *msh)
{
    if (msh->envp)
        free_arg(msh->envp);
    if (msh->ex_envp)
        free_arg(msh->ex_envp);
    if (msh->cmds)
    {
        if (msh->cmds->av)
            free_arg(msh->cmds->av);
        free(msh->cmds);
    }
    if (msh->line)
        free(msh->line);
    free_vector_2(&msh->tokens, msh);
    if (msh->heredoc.fd_heredoc_path)
    {
		free_hd(&msh->heredoc);
        // free(msh->heredoc.fd_heredoc_path);
		free(msh->heredoc.next);
        msh->heredoc.fd_heredoc_path = NULL;
    }
    // if (msh->heredoc.eof)
    // {
    //     free(msh->heredoc.eof);
    //     msh->heredoc.eof = NULL;
    // }
}

// static void	free_all_heredoc(t_msh *msh)
// {
// 	free_arg(msh->envp);
// 	free_arg(msh->ex_envp);
// 	free_arg(msh->cmds->av);
// 	free(msh->line);
// 	free(msh->cmds);
// 	free_vector(&msh->tokens);
// 	if (msh->heredoc.fd_heredoc_path)
// 		free_heredoc(&msh->heredoc);
// }

static void	heredoc_child_process(char *eof, char *temp_path,
								  t_msh *msh, t_tkn *tokens)
{
	int		status;
	int		temp_fd;

	(void) tokens;
	// (void) msh;
	set_signals_to_here_doc();
	while (1)
	{
		temp_fd = open(temp_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		status = fill_fd_heredoc(temp_fd, eof);
		if (status == 1)
			printf("msh: warning: here-document delimited\n");
		close(temp_fd);
		free_all_heredoc(msh);
		if (status == 0 || status == 1)
			exit (0);
		else if (status == 2)
			exit(130);
	}
}

static char	*find_eof(t_tkn *tokens)
{
	t_tkn	*tmp;

	tmp = tokens;
	while (tmp->type != TKN_HEREDOC)
		tmp = tmp->next;
	if (tmp->next)
		return (tmp->next->name);
	return (NULL);
}

static void	transform(t_tkn *tokens, char *path)
{
	t_tkn	*tkn;

	tkn = tokens;
	while (tkn->type != TKN_HEREDOC)
		tkn = tkn->next;
	tkn->type = TKN_IN;
	tkn = tkn->next;
	if (tkn)
	{
		tkn->type = TKN_REDIR_ARG;
		tkn->name = ft_strdup(path);
	}
}

static void	assign_heredoc(t_heredoc **heredoc, char *eof)
{
	t_heredoc	*tmp_hd;

	tmp_hd = *heredoc;
	while (tmp_hd->next)
		tmp_hd = tmp_hd->next;
	tmp_hd->next = ft_calloc(1, sizeof(t_heredoc));
	tmp_hd->next->i = tmp_hd->i + 1;
	tmp_hd->next->count_hd = tmp_hd->count_hd;
	tmp_hd->next->eof = ft_strdup(eof);
	tmp_hd->next->next = NULL;
}

static void	start_heredoc(t_msh *msh, t_tkn *tokens)
{
	t_tkn		*tkn;
	t_heredoc	*hd;

	tkn = tokens;
	hd = &msh->heredoc;
	while (tkn)
	{
		if (tkn->next && tkn->type == TKN_HEREDOC)
		{
			hd = &msh->heredoc;
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

static int	heredoc_len(t_heredoc *heredoc)
{
	int		i;

	i = 0;
	while (heredoc)
	{
		i++;
		heredoc = heredoc->next;
	}
	return (i);
}

void	heredoc_exec(t_msh *msh, t_tkn *tokens)
{
	pid_t		pid;
	char		*i;
	int			status;
	int			len;
	t_heredoc	*heredoc;

	start_heredoc(msh, tokens);
	heredoc = &msh->heredoc;
	msh->heredoc.len = heredoc_len(heredoc);
	len = heredoc_len(heredoc);
	while (heredoc && len--)
	{
		i = ft_itoa(heredoc->i);
		heredoc->fd_heredoc_path = ft_strjoin("/tmp/tmp_heredoc", i);
		free(i);
		pid = fork();
		if (pid == 0)
			heredoc_child_process(heredoc->eof, heredoc->fd_heredoc_path, msh, tokens);
		waitpid(pid, &status, 0);
		transform(tokens, heredoc->fd_heredoc_path);
		heredoc = heredoc->next;
	}
}
