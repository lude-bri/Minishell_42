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

// static void	check_heredoc(t_tkn *tokens, t_msh *msh)
// {
// 	(void)msh;
// 	if (tokens->type == TKN_HEREDOC)
// 		printf("heredoc in the beginning\n");
// 	else
// 		printf("not heredoc in the beginning\n");
// }

// Flag global para indicar interrupção do heredoc
static volatile int g_heredoc_interrupted = 0;

static void	handle_signal(int sig)
{
	if (sig == SIGINT)
	{
		rl_replace_line("", 0); // Limpa a linha atual
		rl_on_new_line();
		// rl_redisplay();         // Atualiza o prompt
		g_heredoc_interrupted = 1; // Sinaliza para sair do loop
	}
}

int heredoc_pipe(t_tkn *tokens, t_msh *msh, char *arg, int flag)
{
    int pipe_fd[2];
    char *line = NULL;
	(void)flag;

    if (pipe(pipe_fd) < 0)
    {
        perror("pipe");
        free_msh(msh->cmds, msh, tokens);
        exit(EXIT_FAILURE);
    }
    signal(SIGINT, handle_signal);
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
            break;

        write(pipe_fd[1], line, ft_strlen(line));
        write(pipe_fd[1], "\n", 1);
        free(line);
    }
	if (flag == 0) //if heredoc is in the beginning, dont dup2
		dup2(pipe_fd[0], STDIN_FILENO);

    free(line);
    close(pipe_fd[1]); // Close write-end after writing
    signal(SIGINT, SIG_DFL);
	free_arg(msh->envp);
	// free_msh(msh->cmds, msh, tokens);
    return (pipe_fd[0]); // Return the read-end for input redirection
}

// int heredoc(t_tkn *tokens, t_msh *msh, char *arg, int flag)
// {
//     int     pipe_fd[2];
//     char    *line = NULL;
//
//     if (pipe(pipe_fd) < 0)
//     {
//         perror("pipe");
//         free_msh(msh->cmds, msh, tokens);
//         exit(EXIT_FAILURE);
//     }
//
//     signal(SIGINT, handle_signal);
//
//     while (1)
//     {
//         if (g_heredoc_interrupted)
//         {
//             close(pipe_fd[0]);
//             close(pipe_fd[1]);
//             free_msh(msh->cmds, msh, tokens);
//             exit(130);
//         }
//
//         line = readline("> ");
//         if (!line || ft_strcmp(line, arg) == 0)
//             break;
//
//         write(pipe_fd[1], line, ft_strlen(line));
//         write(pipe_fd[1], "\n", 1);
//         free(line);
//         line = NULL;
//     }
//
//     free(line);
//     close(pipe_fd[1]);
//     signal(SIGINT, SIG_DFL);
// 	if (flag == 0) //if heredoc is in the beginning, dont dup2
// 		dup2(pipe_fd[0], STDIN_FILENO);
// 	close(pipe_fd[0]);
//     return (pipe_fd[0]); // Return the read end of the pipe
// }

void	heredoc(t_tkn *tokens, t_msh *msh, char *arg, int flag)
{
	int		pipe_fd[2];
	char	*line = NULL;

	if (pipe(pipe_fd) < 0)
	{
		perror("pipe");
		free_msh(msh->cmds, msh, tokens);
		exit(EXIT_FAILURE);
	}
	signal(SIGINT, handle_signal);
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
			break;
		write(pipe_fd[1], line, ft_strlen(line));
		write(pipe_fd[1], "\n", 1);
		free(line);
		line = NULL;
	}
	free(line);
	close(pipe_fd[1]);
	if (flag == 0) //if heredoc is in the beginning, dont dup2
		dup2(pipe_fd[0], STDIN_FILENO);
	close(pipe_fd[0]);
	signal(SIGINT, SIG_DFL);
}

// static void	handle_signal(int sig)
// {
// 	// get_signal(sig+128);
// 	if (sig == SIGINT)
// 	{
// 		rl_replace_line("", 0);
// 		rl_on_new_line();
// 	}
// }
//
// void	heredoc(t_tkn *tokens, t_msh *msh, char *arg)
// {
// 	int		pipe_fd[2];
// 	char	*line;
//
// 	check_heredoc(tokens, msh);
// 	if (pipe(pipe_fd) < 0)
// 	{
// 		perror("pipe");
// 		free_msh(msh->cmds, msh, tokens);
// 		exit(EXIT_FAILURE);
// 	}
// 	while (1)
// 	{
// 		signal(SIGINT, SIG_IGN);
// 		signal(SIGINT, handle_signal);
// 		line = readline("> ");
// 		if (!line || ft_strcmp(line, arg) == 0)
// 			break ;
// 		write(pipe_fd[1], line, ft_strlen(line));
// 		write(pipe_fd[1], "\n", 1);
// 		free(line);
// 	}
// 	free(line);
// 	close(pipe_fd[1]);
// 	dup2(pipe_fd[0], STDIN_FILENO);
// 	close(pipe_fd[0]);
// }
