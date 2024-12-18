/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   420_exec_one.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 11:40:02 by luigi             #+#    #+#             */
/*   Updated: 2024/11/06 11:41:54 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// static void	restruct_cl(t_tkn *tokens, t_msh *msh)
// {
// 	int		fd;
// 	//function to restructure the command line
// 	//organize command "redir" file
// 	(void)tokens;
// 	(void)msh;
// 	printf("restruct redir\n");
// 	fd = open(tokens->next->name, O_RDONLY);
// 	if (fd < 0)
// 		perror(tokens->next->name);
// }

static int	is_bi(t_tkn *tokens)
{
	if (tokens->cmd_type == CMD_CD)
		return (SUCCESS);
	if (tokens->cmd_type == CMD_PWD)
		return (SUCCESS);
	if (tokens->cmd_type == CMD_ENV)
		return (SUCCESS);
	if (tokens->cmd_type == CMD_ECHO)
		return (SUCCESS);
	if (tokens->cmd_type == CMD_EXIT)
		return (SUCCESS);
	if (tokens->cmd_type == CMD_UNSET)
		return (SUCCESS);
	if (tokens->cmd_type == CMD_EXPORT)
		return (SUCCESS);
	else
		return (FAILURE);
}

int	exec_one(t_msh *msh, t_tkn *tokens)
{
	int		pid;
	int		status;

	if (tokens->type == TKN_CMD)
	{
		if (is_bi(tokens) == SUCCESS)
		{
			if (tokens->cmd_type == CMD_EXIT)
				exec_bi(tokens, msh);
			else
			{
				pid = fork();
				if (pid == 0)
				{
					exec_bi(tokens, msh);
					exit(msh->exit_status);
				}
				else if (pid > 0)
				{
					waitpid(pid, &status, 0);
					if (WIFEXITED(status)) // Check if child exited normally
						msh->exit_status = WEXITSTATUS(status); // Update exit status
					else if (WIFSIGNALED(status)) // Handle signals (if needed)
						msh->exit_status = 128 + WTERMSIG(status); // Si
				}
			}
		}
		else
			exec_exe(tokens, msh);
	}
	else if ((tokens->type == TKN_IN || tokens->type == TKN_OUT
		|| tokens->type == TKN_APPEND || tokens->type == TKN_HEREDOC))
	{
		if (tokens->type == TKN_HEREDOC && tokens->next != NULL)
		{	
			heredoc(tokens, msh, tokens->next->name, 1);
			return (SUCCESS);
		}
		if (tokens->next != NULL)
			exec_exe(tokens, msh);
	}
	return (SUCCESS);
}

// int	exec_one(t_msh *msh, t_tkn *tokens)
// {
// 	if (tokens->type == TKN_CMD)
// 		if (exec_bi(tokens, msh) == FAILURE)
// 			if (exec_exe(tokens, msh) != SUCCESS)
// 				return (FAILURE);
// 	return (SUCCESS);
// }
