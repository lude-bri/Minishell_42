/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   510_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:48:25 by luigi             #+#    #+#             */
/*   Updated: 2024/10/26 13:02:19 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	msh_cd(char **argv)
{
	if (argv[1] == NULL)
	{
		printf("cd: missing argument\n");
		return ;
	}
	if (chdir(argv[1]) != 0)
		perror("cd error\n");
}

static int	change_directory(t_msh *msh, char *path)
{
	char	*old;
	int		chdir_ret;

	old = NULL;
	if (path == NULL || msh->pwd == NULL)
	{
		free(msh->pwd);
		pwd = getcwd(NULL, 0);
	}

}

