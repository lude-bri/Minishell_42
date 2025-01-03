/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   100_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:03:56 by luigi             #+#    #+#             */
/*   Updated: 2024/11/11 11:36:40 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void	free_heredoc(t_heredoc *heredoc)
{
    t_heredoc *tmp;

    while (heredoc)
    {
        tmp = heredoc->next; // Save the next node

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
		heredoc->i = 0;
        free(heredoc->next); // Free the current node
		heredoc->next = NULL;
        heredoc = tmp; // Move to the next node
    }
}

int	main(int ac, char **av, char **envp)
{
	t_msh		msh;

	(void)ac;
	(void)av;
	ft_memset(&msh, 0, sizeof(t_msh));
	init_msh(&msh, envp);
	msh_loop(&msh);
	return (free_array(msh.envp, 0));
}

int	msh_loop(t_msh *msh)
{
	t_tkn	*tokens;

	tokens = NULL;
	while (1)
	{
		setup_signals();
		init_struct(msh);
		tokens = to_parse(msh);
		if (!tokens)
			continue ;
		if (msh->cmd_count > NO_CMDS)
			if (!to_execute(msh, tokens))
				break ;
		free_heredoc(&msh->heredoc);
		free_msh(msh->cmds, msh, tokens);
	}
	return (0);
}
