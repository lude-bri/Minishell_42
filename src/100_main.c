/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   100_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:03:56 by luigi             #+#    #+#             */
/*   Updated: 2024/10/18 14:11:14 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int ac, char **av, char **envp)
{
	t_msh		msh;

	(void)ac;
	(void)av;
	msh.envp = envp;
	msh_loop(&msh);
	return (free_array(msh.envp, 0));
}

int	msh_loop(t_msh *msh)
{
	char	*prompt;
	char	**argv;

	prompt = NULL;
	while (1)
	{
		init_struct(msh);
		prompt = readline("Minishell $> ");
		if (*prompt)
			add_history(prompt);
		argv = split_input(prompt);
		printf("%s\n", prompt);
		if (argv[0] && ft_strncmp(argv[0], "exit", 4) == 0)
			msh_exit(argv);
		if (argv[0] && ft_strncmp(argv[0], "pwd", 3) == 0 && ft_strlen(argv[0]) == 3)
			msh_pwd();
		free(prompt);
		free_arg(argv);
	}
	return (0);
}
