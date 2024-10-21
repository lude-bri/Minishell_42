#include "../includes/minishell.h"

void	msh_echo(char **argv)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	if (argv[1] && ft_strncmp(argv[1], "-n", 2) == 0)
	{
		newline = 0;
		i = 2;
	}
	while (argv[i])
	{
		ft_printf("%s", argv[i]);
		if (argv[i + 1])
			ft_printf(" ");
		i++;
	}
	if (newline)
		ft_printf("\n");
}
