#include "../includes/minishell.h"

int	is_num(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	msh_exit(char **argv)
{
	int	exit_status;

	exit_status = 0;
	if (argv[1])
	{
		if (is_num(argv[1]))
			exit_status = ft_atoi(argv[1]);
		else
		{
			printf("Minishell: exit: %s:", argv[1]);
			ft_putstr_fd("numeric argument necessary\n", 2);
			exit_status = 255;
		}
	}
	exit (exit_status);
}
