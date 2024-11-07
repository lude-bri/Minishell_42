/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   100_main.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 13:03:56 by luigi             #+#    #+#             */
/*   Updated: 2024/10/31 17:32:46 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(int ac, char **av, char **envp)
{
	t_msh		msh;

	(void)ac;
	(void)av;
	ft_memset(&msh, 0, sizeof(t_msh));
	init_msh(&msh, envp);
	msh_loop(&msh);
	return (free_array(msh.envp, 0));
}

int	msh_loop(t_msh *msh)
{
	char	*line;
	t_tkn	*tokens;

	tokens = NULL;
	while (1)
	{
		setup_signals();
		init_struct(msh);
		line = readline("msh $ ");
		if (!line)
		{
			ft_printf("exit\n");
			break ;
		}
		tokens = to_parse(msh, line);
		if (!tokens)
			continue ;
		msh->tree_head = create_bin_tree(tokens);
		if (msh->cmd_count > NO_CMDS)
			if (!to_execute(msh->cmds->av, msh, tokens))
				break ;
		free_msh(msh->cmds, msh, tokens);
	}
	return (0);
}


/////////////////////////////
///   OLD LOOP FUNCT      ///
////////////////////////////

// int	msh_loop(t_msh *msh)
// {
// 	char	*prompt;
// 	char	**argv;
// 	int		i; //to test quotes (remember to delete later)
//
// 	prompt = NULL;
// 	setup_signals();
// 	while (1)
// 	{
// 		init_struct(msh);
// 		prompt = readline("msh $ ");
// 		//End Of Input(EOI) (ctrl-D)
// 		if (!prompt)
// 		{
// 			ft_printf("exit\n");
// 			break ;
// 		}
// 		if (*prompt)
// 			add_history(prompt);
// 		argv = split_input(prompt);
// 		i = 0;
// 		while (argv[i])
// 		{
// 			ft_printf("Arg number %d: %s\n", i, argv[i]);
// 			//free(arv[i]);
// 			i++;
// 		}
// 		if (argv[0] && ft_strcmp(argv[0], "exit") == 0)
// 			msh_exit(argv, msh);
// 		if (argv[0] && ft_strcmp(argv[0], "pwd") == 0 && ft_strlen(argv[0]) == 3)
// 			msh_pwd();
// 		if (argv[0] && ft_strcmp(argv[0], "echo") == 0)
// 			msh_echo(argv);
// 		if (argv[0] && ft_strcmp(argv[0], "cd") == 0)
// 			msh_cd(argv);
// 		if (argv[0] && ft_strcmp(argv[0], "export") == 0)
// 			msh_export(msh->envp);
// 		if (argv[0] && ft_strcmp(argv[0], "env") == 0)
// 			msh_env(msh->envp);
// 		if (argv[0] && ft_strcmp(argv[0], "unset") == 0)
// 			msh_unset(argv, &(msh->envp));
// 		if (prompt)
// 		{
// 			free(prompt);
// 			prompt = NULL;
// 		}
// 		free_arg(argv);
// 	}
// 	return (0);
// }
