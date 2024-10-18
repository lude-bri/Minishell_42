
#ifndef MINISHELL_H
# define MINISHELL_H


/* ************************************************************************** */
/*                                  INCLUDES                                  */
/* ************************************************************************** */

# include <unistd.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../lib/libft/libft_42/libft.h"

/* ************************************************************************** */
/*                                  DEFINES                                   */
/* ************************************************************************** */

# define INT_MAX 2147483647 
# define INT_MIN -2147483647

/* ************************************************************************** */
/*                                  STRUCTS                                   */
/* ************************************************************************** */

//struct that deals with commands
typedef struct s_command 
{	
	int		ac; //argument counter
	char	**av; //argument values
	char	*cmd; //command
}			t_command;

//main struct to deal with the hole minishell system
typedef struct s_shell
{
	t_command	*cmds;
	int			cmd_count;
	int			exit_status;
	char		**envp;
	char		**path;
	int			shell_level;
}				t_shell;

/* ************************************************************************** */
/*                                 FUNCTIONS                                  */
/* ************************************************************************** */


#endif
