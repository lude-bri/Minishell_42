/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   120_prompt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 12:23:10 by luigi             #+#    #+#             */
/*   Updated: 2024/10/19 12:44:33 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	build_prompt(t_msh *msh)
{
	//bash prompt is $USER@$SESSION_MANAGER:$PWD
	char	*user;
	int		i;
	int		j;

	i = 0;
	user = NULL;
	if (msh->envp[i])
		return (0);
	while (ft_strnstr(msh->envp[i], "USER=", 5) == 0)
		i++;
	j = 0;
	while (msh->envp[i])
	{
		user[j] = msh->envp[i][j];
		i++;
		j++;
	}
	return (0);
}
