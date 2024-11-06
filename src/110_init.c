/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   110_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/18 12:34:09 by luigi             #+#    #+#             */
/*   Updated: 2024/10/31 16:21:30 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static void increment_shlvl(char **envp);
// static char *init_prompt(t_msh *msh)
// {
// 	char	*prompt;
// 	char	*tmp;
//
// 	tmp = ft_strjoin(msh->user, "@");
// 	prompt = ft_strjoin(tmp, msh->hostname);
// 	free(tmp);
// 	tmp = ft_strjoin_free(prompt, ":", 1);
// 	return (tmp);
// }

void	init_msh(t_msh *msh, char **envp)
{
	if (!msh->envp)
		msh->envp = init_env(envp); //get_envp
	// msh->home = get_variable("HOME", envp);
	// msh->user = get_variable("USER", envp);
	// msh->hostname = get_variable("SESSION_MANAGER", envp); 
	// if (!msh->hostname)
	// 	msh->hostname = "42";
	msh->cmds = NULL;
	// msh->prompt = init_prompt(msh);
	increment_shlvl(msh->envp);
}

void	init_struct(t_msh *msh)
{
	msh->path = NULL;
	msh->cmd_count = 0;
	// msh->exit_status = 0;
}

char	**init_env(char **envp)
{
	char	**env_copy;
	int		i;

	i = 0;
	while (envp[i])
		i++;
	env_copy = malloc(sizeof(char *) * (i + 1));
	if (!env_copy)
		return (NULL);
	i = -1;
	while (envp[++i])
	{
		env_copy[i] = ft_strdup(envp[i]);
		if (!env_copy)
			return (NULL);
	}
	env_copy[i] = NULL;
	return (env_copy);
}

static void increment_shlvl(char **envp)
{
    int		i;
    int		shlvl_value;
    char	*new_shlvl;
    char	*shlvl_str;

	i = 0;
    while (envp[i])
    {
        if (ft_strnstr(envp[i], "SHLVL=", 6))
        {
            shlvl_value = ft_atoi(envp[i] + 6);
            shlvl_value++;
            shlvl_str = ft_itoa(shlvl_value);
            new_shlvl = ft_strjoin("SHLVL=", shlvl_str);
            free(envp[i]);
            envp[i] = new_shlvl;
            free(shlvl_str);
            return;
        }
        i++;
    }
    envp[i] = ft_strdup("SHLVL=1");
    envp[i + 1] = NULL;
}
