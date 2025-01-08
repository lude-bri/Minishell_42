/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   520_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:49:08 by luigi             #+#    #+#             */
/*   Updated: 2024/12/23 00:37:45 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	verify_pwd_args(char **av)
{
	while (ft_strcmp(*av, "pwd") != 0)
		av++;
	av++;
	if (ft_strncmp(*av, "-", 1) == 0)
	{
		write(2, "msh: invalid option\n", 21);
		return (1);
	}
	return (0);
}

int	msh_pwd(t_msh *msh, t_tkn *tokens)
{
	char	*cwd;
	int		found;

	(void)tokens;
	found = 0;
	if (msh->cmds->av[1])
		found = verify_pwd_args(msh->cmds->av);
	if (found == 1)
		return (1);
	cwd = getcwd(NULL, 0);
	if (cwd != NULL)
	{
		write(STDOUT_FILENO, cwd, ft_strlen(cwd));
		write(STDOUT_FILENO, "\n", 1);
		free(cwd);
		return (1);
	}
	else
	{
		perror("pwd");
		return (1);
	}
	return (0);
}
