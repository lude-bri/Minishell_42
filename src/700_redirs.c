/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   700_redirs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: luigi <luigi@student.42porto.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/16 09:09:40 by luigi             #+#    #+#             */
/*   Updated: 2024/11/18 10:44:16 by luigi            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// static void	redirs_2(t_tkn *tokens, t_msh *msh);

static void	flags_check(t_tkn *tokens)
{
	if (ft_strchr(tokens->next->next->name, '-') != 0)
		printf("and it is exec\n");
	else
		printf("and it is not exec\n");
}

void	redirs(t_tkn *tokens, t_msh *msh)
{
	t_tkn	*tkn_exec;

	tkn_exec = tokens;
	if (tokens->next->next != NULL && tokens->next->next->type != TKN_PIPE)
	{	
		printf("there is something\n");
		flags_check(tokens);
	}
    if (tokens->type == TKN_IN)
		redir_in(tkn_exec, msh);
    else if (tokens->type == TKN_OUT)
		redir_out(tkn_exec, msh);
    else if (tokens->type == TKN_APPEND)
		redir_append(tkn_exec, msh);
}

//o que estiver imediatamante apos os redirs vai ser um
//TKN_REDIR_ARG, e tudo a seguir disso vai ser guardado
//em um char** para ser enviado para o execve
void	redirs_2(t_tkn *tokens, t_msh *msh)
{
	// char	**arg;
	int			i;

	i = 0;
	msh->arg = malloc(sizeof(char *) * (tokens->len + 1));
	while (tokens)
	{
		if (tokens->type == TKN_PIPE)
			break ;
		if (tokens->type == TKN_IN || tokens->type == TKN_OUT 
			|| tokens->type == TKN_APPEND)
		{
			if (tokens->type == TKN_IN)
				redir_in(tokens, msh);
			else if (tokens->type == TKN_OUT)
				redir_out(tokens, msh);
			else if (tokens->type == TKN_APPEND)
			{
				redir_append(tokens, msh);
				tokens->next->next->type = TKN_REDIR_ARG;
			}
			if (tokens->next != NULL)
			{
				if (tokens->next->type == TKN_PIPE)
					break;
				else
					tokens->next->type = TKN_REDIR_ARG;
			}
		}
		if (tokens->type == TKN_CMD && tokens->type != TKN_REDIR_ARG)
		{
			msh->arg[i] = ft_strdup(tokens->name);
			i++;
		}
		tokens = tokens->next;
	}
	msh->arg[i] = NULL;
	free_arg(msh->cmds->av);
	msh->cmds->av = msh->arg;
	// free_arg(tokens->cmdargs);
	// tokens->cmdargs = NULL;
	// tokens->cmdargs = msh->arg;
	msh->flag_redir = true;
}


// int exec_redirs(t_tkn *tokens, t_msh *msh)
// {
//     t_tkn *tkn;
//
//     tkn = tokens;
//     while (tkn)
//     {
//         if (tkn->type == TKN_HEREDOC)
//         {
//             heredoc(tokens, msh, tkn->next->name, 0);
//             return (SUCCESS);
//         }
//         if (tkn->type == TKN_IN || tkn->type == TKN_OUT 
// 			|| tkn->type == TKN_APPEND)
//         {
//             if (tkn->type == TKN_IN)
//                 redir_in(tkn, msh);
//             else if (tkn->type == TKN_OUT)
//                 redir_out(tkn, msh);
//             else if (tkn->type == TKN_APPEND)
//                 redir_append(tkn, msh);
//         }
//         tkn = tkn->next;
//     }
//     return (SUCCESS);
// }

static int	redir_found(t_tkn *tokens)
{
	return ((tokens->type == TKN_IN || tokens->type == TKN_OUT
		|| tokens->type == TKN_APPEND));
}

// int exec_redirs(t_tkn *tokens, t_msh *msh)
// {
//     t_tkn *tkn;
//
//     tkn = tokens;
//     while (tkn)
//     {
//         if (tkn->type == TKN_HEREDOC)
//         {
//             heredoc(tokens, msh, tkn->next->name, 0);
//             if (tkn->next)
//                 tkn = tkn->next;
//         }
//         else if (tkn->type == TKN_IN || tkn->type == TKN_OUT || tkn->type == TKN_APPEND)
//         {
//             redirs_2(tkn, msh);
//             if (tkn->next && tkn->next->type == TKN_REDIR_ARG)
//                 tkn = tkn->next;
//         }
//         tkn = tkn->next;
//     }
//     return (SUCCESS);
// }

int	exec_redirs(t_tkn *tokens, t_msh *msh)
{
	t_tkn	*tkn;

	tkn = tokens;
	while (tkn)
	{
		if (tkn->type == TKN_HEREDOC && !redir_found(tkn))
		{
			heredoc(tokens, msh, tkn->next->name, 0);
			return (SUCCESS);
		}
		if (tkn->type == TKN_IN || tkn->type == TKN_OUT
			|| tkn->type == TKN_APPEND)
		{
			redirs_2(tokens, msh);
		}
		tkn = tkn->next;
	}
	return (SUCCESS);
}
