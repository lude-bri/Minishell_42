/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:48:25 by luigi             #+#    #+#             */
/*   Updated: 2024/10/23 12:48:27 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	msh_cd(char **argv)
{
	if (argv[1] == NULL)
	{
		ft_printf("cd: missing arument\n");
		return ;
	}
	if (chdir(argv[1]) != 0)
		perror("cd error\n");
}
