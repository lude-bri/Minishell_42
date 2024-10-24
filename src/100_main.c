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
	//init minishell
	ft_memset(&msh, 0, sizeof(t_msh));
	init_msh(&msh, envp);
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
		// prompt = readline("Minishell $> ");
		prompt = readline(build_prompt(msh)); 
		if (*prompt)
			add_history(prompt);
		argv = split_input(prompt);
		//argv = ft_split(prompt, ' ');
		//printf("%s\n", prompt);
		if (argv[0] && ft_strncmp(argv[0], "exit", 4) == 0)
			msh_exit(argv);
		if (argv[0] && ft_strncmp(argv[0], "pwd", 3) == 0 && ft_strlen(argv[0]) == 3)
			msh_pwd();
		if (argv[0] && ft_strncmp(argv[0], "echo", 4) == 0)
			msh_echo(argv);
		if (argv[0] && ft_strncmp(argv[0], "cd", 2) == 0)
			msh_cd(argv);
		if (argv[0] && ft_strncmp(argv[0], "export", 6) == 0)
			msh_export(msh->envp);
		if (argv[0] && ft_strncmp(argv[0], "env", 3) == 0)
			msh_env(msh->envp);
		free(prompt);
		free(msh->hostname);
		free_arg(argv);
	}
	return (0);
}
