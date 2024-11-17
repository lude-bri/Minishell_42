/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   700_redirs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 09:09:40 by luigi             #+#    #+#             */
/*   Updated: 2024/11/16 10:20:58 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void heredoc_handler(t_tkn *tokens, t_msh *msh);

static void redirs(t_tkn *tokens, t_msh *msh)
{
    int		fd;
	t_tkn	*tkn_exec;

	tkn_exec = tokens;
	//input redir (<)
    if (tokens->type == TKN_IN)
    {
		tkn_exec = tkn_exec->next;
        fd = open(tkn_exec->name, O_RDONLY);
        if (fd == -1)
        {
            perror(tkn_exec->name);
            free_msh(msh->cmds, msh, tkn_exec);
            exit(1);
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
	//output redir (>)
    else if (tokens->type == TKN_OUT)
    {
		tkn_exec = tkn_exec->next;
        fd = open(tkn_exec->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1)
        {
            perror(tkn_exec->name);
            free_msh(msh->cmds, msh, tkn_exec);
            exit(1);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
	//append redir (>>)
    else if (tokens->type == TKN_APPEND)
    {
        fd = open(tokens->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd == -1)
        {
            perror(tokens->name);
            free_msh(msh->cmds, msh, tokens);
            exit(1);
        }
        dup2(fd, STDOUT_FILENO);
        close(fd);
    }
	//heredoc (<<)
	if (tokens->type == TKN_HEREDOC)
        heredoc_handler(tokens, msh);
}

static void heredoc_handler(t_tkn *tokens, t_msh *msh)
{
    int		pipe_fd[2];
    char	*line;

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        free_msh(msh->cmds, msh, tokens);
        exit(1);
    }
    while (1)
    {
        line = readline("> ");
        if (!line || ft_strcmp(line, tokens->next->name) == 0)
            break;
        write(pipe_fd[1], line, ft_strlen(line));
        write(pipe_fd[1], "\n", 1);
        free(line);
    }
    free(line);
    close(pipe_fd[1]);
    dup2(pipe_fd[0], STDIN_FILENO);
    close(pipe_fd[0]);
}

int	exec_redirs(t_tkn *tokens, t_msh *msh)
{
	//printf("redirec activated\n");
	while (tokens)
	{
		if (tokens->type == TKN_HEREDOC)
		{
			heredoc_handler(tokens, msh);
			return (SUCCESS);
		}
		if (tokens->type == TKN_IN || tokens->type == TKN_OUT)
		{
			redirs(tokens, msh);
			return (SUCCESS);
		}
		tokens = tokens->next;
	}
	return (SUCCESS);
}


