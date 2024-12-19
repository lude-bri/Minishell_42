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

int find_env_var(char **envp, const char *var)
{
    int i = 0;
    size_t var_len = strlen(var);
    
    while (envp[i])
    {
        if (strncmp(envp[i], var, var_len) == 0 && envp[i][var_len] == '=')
            return (i);
        i++;
    }
    return (-1);
}

// Helper function to update environment variable
void update_env_var(char **envp, const char *var, const char *value)
{
    int index;
    char *new_var;
    
    // Calculate the length needed and allocate
    new_var = malloc(strlen(var) + strlen(value) + 2); // +2 for '=' and '\0'
    if (!new_var)
        return;
        
    // Create the new environment string
    sprintf(new_var, "%s=%s", var, value);
    
    // Find if variable already exists
    index = find_env_var(envp, var);
    
    if (index != -1)
    {
        // Free the old string and replace it
        free(envp[index]);
        envp[index] = new_var;
    }
}

int msh_cd(char **argv, char **envp)
{
    char current_path[PATH_MAX];
    char new_path[PATH_MAX];
    
    if (!argv || !envp)
        return (1);
        
    // Get current path before changing directory
    if (getcwd(current_path, sizeof(current_path)) == NULL)
    {
        perror("msh: getcwd");
        return (1);
    }
    
    // Handle cd argument checks
    if (argv[1] == NULL)
    {
        char *home = getenv("HOME");
        if (!home)
        {
            printf("msh: cd: HOME not set\n");
            return (1);
        }
        if (chdir(home) != 0)
        {
            perror("msh: cd");
            return (1);
        }
    }
    else if (argv[2])
    {
        // printf("msh: cd: too many arguments\n");
		write(STDERR_FILENO, " too many arguments\n", 18);
        return (1);
    }
    else if (chdir(argv[1]) != 0)
    {
        perror("msh: cd");
        return (1);
    }
    
    // Get new current working directory
    if (getcwd(new_path, sizeof(new_path)) == NULL)
    {
        perror("msh: getcwd");
        return (1);
    }
    
    // Update PWD and OLDPWD in environment
    update_env_var(envp, "OLDPWD", current_path);
    update_env_var(envp, "PWD", new_path);
    
    return (0);
}

// int	msh_cd(char **argv)
// {
// 	if (argv[1] == NULL)
// 	{
// 		printf("cd: missing argument\n");
// 		return (1);
// 	}
// 	else if (argv[2])
// 	{
// 		printf("msh: cd: too many arguments\n");
// 		return (1);
// 	}
// 	if (chdir(argv[1]) != 0)
// 	{
// 		perror("msh: cd");
// 		return (1);
// 	}
// 	return (0);
// }
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
