/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   510_cd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:48:25 by luigi             #+#    #+#             */
/*   Updated: 2024/10/31 17:09:39 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	msh_cd(char **argv)
{
	if (argv[1] == NULL)
	{
		printf("cd: missing argument\n");
		return (1);
	}
	if (chdir(argv[1]) != 0)
	{
		perror("cd error\n");
		return (1);
	}
	return (0);
}
//
// static int	change_directory(t_msh *msh, char *path)
// {
// 	char	*old;
// 	int		chdir_ret;
//
// 	old = NULL;
// 	if (path == NULL || msh->pwd == NULL)
// 	{
// 		free(msh->pwd);
// 		pwd = getcwd(NULL, 0);
// 	}
//
// }
//
