#include "../includes/minishell.h"

void	msh_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd != NULL)
	{
		ft_printf("%s\n", cwd);
		free(cwd);
	}
	else
		perror("pwd");
}
