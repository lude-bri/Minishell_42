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

static char	*get_pwd(t_msh *msh, char **envp)
{
	char	*new_pwd;
	char	*pwd;
	char	*home;
	int		j = 0;
	int		i;

	pwd = get_variable("PWD", envp);
	new_pwd = ft_calloc(ft_strlen(pwd), sizeof(char));
	home = msh->home;
	i = 0;
	while (pwd[i] == home[i])
		i++;
	new_pwd[j++] = '~';
	while (pwd[i])
	{
		new_pwd[j] = pwd[i];
		i++;
		j++;
	}
	free(pwd);
	return (new_pwd); 
}

char	*build_prompt(t_msh *msh)
{
	char	*pwd;
	char	*prompt;

	pwd = get_pwd(msh, msh->envp);
	prompt = msh->prompt;
	prompt = ft_strjoin(prompt, pwd);
	prompt = ft_strjoin(prompt, " $>");
	return (prompt);
}


