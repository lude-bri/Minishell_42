/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   550_env.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 10:32:18 by luigi             #+#    #+#             */
/*   Updated: 2025/01/08 17:40:08 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	env_command_not_found(t_msh *msh, t_tkn *tkn)
{
	if (tkn->next->type == TKN_CMD)
	{
		printf("env: No such file or directory\n");
		msh->exit_status = 127;
		return (1);
	}
	return (0);
}

int	msh_env(char **envp, t_msh *msh, t_tkn *tkn)
{
	int		i;
	int		j;
	int		status;

	if (!envp)
		return (1);
	status = 0;
	if (msh->cmds->av[1])
		status = env_command_not_found(msh, tkn);
	if (status == 1)
		return (1);
	i = -1;
	while (envp[++i])
	{
		j = 0;
		while (envp[i][j])
		{
			if (write(1, &envp[i][j], 1) == -1)
				return (1);
			j++;
		}
		if (write (1, "\n", 1) == -1)
			return (1);
	}
	return (0);
}
