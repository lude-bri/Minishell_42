/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   740_heredoc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 08:11:30 by luigi             #+#    #+#             */
/*   Updated: 2025/01/08 17:26:57 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Signal handler for heredoc SIGINT (Ctrl+C).
 *
 * Used to gracefully interrupt heredoc input and close stdin,
 * allowing the shell to return to the prompt.
 *
 * @param signal The signal received (only SIGINT handled).
 */
static void	handle_heredoc_signal(int signal)
{
	if (signal == SIGINT)
	{
		ft_putstr_fd("\n", 1);
		g_signal = SIGINT;
		close(STDIN_FILENO);
	}
}

/**
 * @brief Configures signal handling for heredoc child processes.
 *
 * Sets `SIGINT` to a custom handler and restores `SIGQUIT`
 * to its default behavior.
 */
void	set_heredoc_signals(void)
{
	signal(SIGINT, handle_heredoc_signal);
	signal(SIGQUIT, SIG_DFL);
}

/**
 * @brief Reads user input until the EOF delimiter is reached.
 *
 * Called in the child process. Reads lines from stdin and writes
 * them to a temporary file until the user inputs the specified `eof`.
 *
 * @param temp_fd File descriptor to write heredoc content to.
 * @param eof End-of-file delimiter string.
 * @return int 0 on success, 1 on EOF without input, 2 on interrupt (SIGINT).
 */
int	fill_heredoc(int temp_fd, char *eof)
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

/**
 * @brief Initializes heredoc structures from tokens.
 *
 * Iterates over tokens to find heredoc operators (`<<`) and stores
 * their associated EOF markers in the shell's heredoc structure.
 *
 * @param msh Pointer to the shell state.
 * @param tokens Token list containing possible heredoc entries.
 */
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

/**
 * @brief Executes the heredoc functionality (`<<`) for the given tokens.
 *
 * This function prepares and executes heredoc behavior by:
 * - Starting the heredoc tracking structure.
 * - Creating temporary files in `/tmp/` to store heredoc input.
 * - Forking child processes to handle user input via `readline("> ")`.
 * - Replacing heredoc tokens with temporary file redirections.
 *
 * @param msh Pointer to the shell's main state.
 * @param tokens Pointer to the token list being parsed.
 */
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
				msh);
		waitpid(pid, &status, 0);
		transform(tokens, heredoc->fd_heredoc_path);
		heredoc = heredoc->next;
	}
}
