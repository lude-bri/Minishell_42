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
    int fd;

    if (tokens->type == TKN_IN) // Input redirection (<)
    {
        fd = open(tokens->name, O_RDONLY);
        if (fd == -1)
        {
            perror(tokens->name);
            free_msh(msh->cmds, msh, tokens);
            exit(1);
        }
        dup2(fd, STDIN_FILENO); // Replace stdin with fd
        close(fd);
    }
    else if (tokens->type == TKN_OUT) // Output redirection (>)
    {
		tokens = tokens->next;
        fd = open(tokens->name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (fd == -1)
        {
            perror(tokens->name);
            free_msh(msh->cmds, msh, tokens);
            exit(1);
        }
        dup2(fd, STDOUT_FILENO); // Replace stdout with fd
        close(fd);
    }
    else if (tokens->type == TKN_APPEND) // Append redirection (>>)
    {
        fd = open(tokens->name, O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (fd == -1)
        {
            perror(tokens->name);
            free_msh(msh->cmds, msh, tokens);
            exit(1);
        }
        dup2(fd, STDOUT_FILENO); // Replace stdout with fd
        close(fd);
    }
	else if (tokens->type == TKN_HEREDOC) // Heredoc (<<)
    {
        // Handle heredoc logic (example below)
        heredoc_handler(tokens, msh);
    }
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
        line = readline("> "); // Prompt user for input
        if (!line || ft_strcmp(line, tokens->name) == 0) // Delimiter reached
            break;

        write(pipe_fd[1], line, ft_strlen(line));
        write(pipe_fd[1], "\n", 1);
        free(line);
    }
    free(line);
    close(pipe_fd[1]); // Close write end of the pipe
    dup2(pipe_fd[0], STDIN_FILENO); // Replace stdin with read end of the pipe
    close(pipe_fd[0]);
}

int exec_redirs(t_tkn *tokens, t_msh *msh)
{
    t_tkn *current = tokens;

    while (current)
    {
        if (current->type == TKN_IN || current->type == TKN_OUT || 
            current->type == TKN_APPEND || current->type == TKN_HEREDOC)
        {
            redirs(current, msh);
        }
        current = current->next;
    }
    return (SUCCESS);
}

//
//
// int	exec_redirs(t_tkn *tokens, t_msh *msh)
// {
// 	//printf("redirec activated\n");
// 	while (tokens)
// 	{
// 		if (tokens->type == TKN_HEREDOC)
// 		{
// 			printf("heredoc found\n");
// 			heredoc_handler(tokens, msh);
// 			return (SUCCESS);
// 		}
// 		if (tokens->type == TKN_IN || tokens->type == TKN_OUT)
// 		{
// 			redirs(tokens, msh);
// 			return (SUCCESS);
// 		}
// 		tokens = tokens->next;
// 	}
// 	return (FAILURE);
// }


