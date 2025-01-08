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
