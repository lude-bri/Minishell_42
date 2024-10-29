/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   111_init_var.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:55:08 by luigi             #+#    #+#             */
/*   Updated: 2024/10/26 12:49:49 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static char		*find_variable(char *target, char **envp, int flag);
static char		*get_hostname(char *hostname, char c);

char	*get_variable(char *v, char **envp)
{
	char	*find;
	char	*hostname;

	hostname = "SESSION_MANAGER";
	if (v == hostname)
		find = find_variable(v, envp, 1);
	else
		find = find_variable(v, envp, 0);
	return (find);
}

static char	*find_variable(char *target, char **envp, int flag)
{
	char	*tmp;
	char	*var;
	int		len;

	if ((target == NULL) || (envp == NULL))
		return (NULL);
	tmp = ft_strjoin(target, "=");
	len = ft_strlen(tmp);
	var = NULL;
	while (*envp)
	{
		if (ft_strncmp(tmp, *envp, len) == 0)
		{
			var = ft_strdup(*envp + len);
			break ;
		}
		++envp;
	}
	if (flag == 1)
		var = get_hostname(var, '.');
	free(tmp);
	return (var);
}

static char		*get_hostname(char *hostname, char c)
{
	int		i;
	int		j;
	int		start;
	char	*trim;

	i = 0;
	j = 0;
	while (hostname[i] != '/')
		i++;
	start = i;
	while (hostname[i++] != c)
		j++;
	trim = ft_calloc((j + 1), sizeof(char));
	start++;
	j = 0;
	while (hostname[start])
	{
		if (hostname[start] != c)
			trim[j] = hostname[start];
		else
			break ;
		start++;
		j++;
	}
	trim[j] = '\0';
	free(hostname);
	return (trim);
}
