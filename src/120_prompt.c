/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   120_prompt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 12:23:10 by luigi             #+#    #+#             */
/*   Updated: 2025/01/07 19:41:43 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*build_prompt(t_msh *msh)
{
	char	*line;

	line = readline("\001\033[1;32m\002msh$ \001\033[0m\002");
	if (g_signal == 130)
	{
		msh->exit_status = 130;
		g_signal = 0;
	}
	if (!line)
	{
		printf("exit\n");
		free_arg(msh->envp);
		free_arg(msh->ex_envp);
		free(msh->heredoc);
		exit (0);
	}
	else if (line[0] == '\0')
		return (NULL);
	return (line);
}
