
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
//it's called msh because its a MiniSHell
typedef struct s_msh
{
	t_command	*cmds;
	char		**envp;
	char		**path;

	char		*user;
	char		*home;
	char		*hostname;

	int			cmd_count;
	int			exit_status;

}				t_msh;

/* ************************************************************************** */
/*                                 FUNCTIONS                                  */
/* ************************************************************************** */

/* **************** */
/*      100        */
/* *************** */

//100_main.c
int	main(int ac, char **av, char **envp);
int	msh_loop(t_msh *msh);

/* **************** */
/*      110        */
/* *************** */

//110_init.c
void	init_msh(t_msh *msh, char **envp);
void	init_struct(t_msh *msh);
char	**init_env(char **envp);

/* **************** */
/*      900        */
/* *************** */

//900_free.c
int		free_array(char **str, int error);
void	free_arg(char **argv);

//exit.c
int		is_num(const char *str);
void	msh_exit(char **argv);

//tokenization.c
char	**split_input(const char *input);
int	count_words(const char *input);
int	is_whitespace(char c);
char	*copy_word(const char *input, int start, int end);

//pwd.c
void	msh_pwd(void);

//echo.c
void	msh_echo(char **argv);

//cd.c
void	msh_cd(char **argv);

//export.c
void msh_export(char **envp);

//env.c
void	msh_env(char **envp);

#endif
