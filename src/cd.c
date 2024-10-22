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
