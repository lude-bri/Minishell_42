/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   741_heredoc_pipe.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 03:02:43 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/07 17:34:59 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/* int	heredoc_pipe(t_tkn *tokens, t_msh *msh, char *arg, int flag)
{
	int	pipe_fd[2];

	(void)tokens;
	(void)arg;
	if (flag == 0)
		dup2(pipe_fd[0], STDIN_FILENO);
	signal(SIGINT, SIG_DFL);
	free_arg(msh->envp);
	free_arg(msh->ex_envp);
	return (pipe_fd[0]);
} */

void	heredoc_child_process(char *eof, char *temp_path,
								t_msh *msh, t_tkn *tokens)
{
	int		status;
	int		temp_fd;

	(void) tokens;
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
