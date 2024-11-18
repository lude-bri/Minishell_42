/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   520_pwd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:49:08 by luigi             #+#    #+#             */
/*   Updated: 2024/11/18 10:45:08 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	msh_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd != NULL)
	{
		ft_printf("%s\n", cwd);
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
