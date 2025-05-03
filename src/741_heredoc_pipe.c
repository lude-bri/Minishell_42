/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   741_heredoc_pipe.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 03:02:43 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/08 17:27:54 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Handles heredoc input collection in a child process.
 *
 * This function is executed in a forked child process.
 * It opens a temporary file (at `temp_path`), reads user input via
 * `fill_heredoc()` until the EOF delimiter is reached, and stores the
 * heredoc content in the file.
 *
 * It handles:
 * - Signal setup (e.g., `SIGINT`).
 * - Temporary file creation/truncation.
 * - Handling different exit statuses (normal completion, interrupt).
 *
 * @param eof The EOF delimiter string (e.g., "EOF" in `<< EOF`).
 * @param temp_path Path to the temporary file used to store heredoc input.
 * @param msh Pointer to the shell’s state (`t_msh`), used to free memory.
 */
void	heredoc_child_process(char *eof, char *temp_path, t_msh *msh)
{
	int		status;
	int		temp_fd;

	set_heredoc_signals();
	while (1)
	{
		temp_fd = open(temp_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		status = fill_heredoc(temp_fd, eof);
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
