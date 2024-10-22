#include "../includes/minishell.h"

void	msh_export(char **envp)
{
	int		i;
	int		j;
	char	*equal_sign;

	i = 0;
	while (envp[i])
	{
		equal_sign = ft_strchr(envp[i], '=');
		j = 0;
		if (equal_sign)
		{
			while (&envp[i][j] != equal_sign)
			{
				write(1, &envp[i][j], 1);
				j++;
			}
			ft_printf("=\"%s\"\n", equal_sign + 1);
		}
		i++;
	}
}
