
#ifndef MINISHELL_H
# define MINISHELL_H
# define _GNU_SOURCE

/* ************************************************************************** */
/*                                  INCLUDES                                  */
/* ************************************************************************** */

# include <unistd.h>
# include <fcntl.h>
# include <stdio.h>
# include <signal.h>
# include <sys/wait.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../lib/libft/libft_42/libft.h"

/* ************************************************************************** */
/*                                  DEFINES                                   */
/* ************************************************************************** */

//Max
# define INT_MAX 2147483647 
# define INT_MIN -2147483647

//Error
# define INIT_ERROR			"msh: Init Error\n"
# define ENV_INIT_ERROR		"msh: Env Init Error\n"
# define ENV_VAR_ERROR		"msh: Env Variable Error\n"
# define TKN_ERROR			"msh: Token Error\n"
# define TKNZ_ERROR			"msh: Tokenizer Error\n"
# define PRS_ERROR			"msh: Parse Error\n"
# define PRSN_ERROR			"msh: Parsing Error\n"
# define MALLOC_ERROR		"msh: Malloc Error\n"
# define CMD_ERROR			"msh: Command not found\n"
# define ARG_ERROR			"msh: Invalid Arguments\n"

//Semantics
# define NO_ENV -1
# define NO_VAR -1
# define NO_TOKEN 0
# define NO_PATH 0
# define NO_CMDS 0


/* ************************************************************************** */
/*                                  ENUMS								  	  */
/* ************************************************************************** */

//Minishell Status
typedef enum e_status 
{
	FAILURE,
	SUCCESS
}   t_status;

//Token Groups
typedef	enum e_token_group
{
	TKN_IN,
	TKN_APPEND,
	TKN_OUT,
	TKN_CMD,
	TKN_NULL,
	TKN_BLANK,
	TKN_PIPE,
	TKN_HEREDOC
}	t_token_group;

//Command Groups
typedef enum e_cmd_group
{
	CMD_EXEC,
	CMD_ECHO,
	CMD_CD,
	CMD_PWD,
	CMD_EXPORT,
	CMD_UNSET,
	CMD_ENV,
	CMD_CLEAR,
	CMD_EXIT,
	CMD_PATH
}	t_cmd_group;

/* ************************************************************************** */
/*                                  STRUCTS                                   */
/* ************************************************************************** */


//struct to deal with operations when tokenizing
//used in 200_tokenization.c
typedef struct s_tkn_op 
{
	int		number_words;
	int		start;
	char	**argv;
}			t_tkn_op;

//struct to deal with tokens
typedef struct s_tkn 
{
	t_token_group	type;
	t_cmd_group		cmd_type;
	char			*name;
	char			**cmdargs; // need to create
	int				len;
	struct s_tkn	*tokens;
	struct s_tkn	*left; //to binary tree
	struct s_tkn	*right; //to binary tree
	struct s_tkn    *next; //linked list -> helps w free
}					t_tkn;

//struct that deals with commands
typedef struct s_command 
{	
	int		ac; //argument counter
	char	**av; //argument values
	char	*cmd; //command
}			t_command;

//struct to realloc and deal with tokens
typedef struct s_vector
{
	int		count;
	int		size;
	t_tkn	**buffer;
}			t_vector;	

//main struct to deal with the hole minishell system
//it's called msh because its a MiniSHell
typedef struct s_msh
{
	t_command	*cmds;
	t_tkn		*tree_head; //binary tree head to execute
	char		**envp;
	char		**path;

	char		*user;
	char		*home;
	char		*hostname;
	char		*pwd;
	char		*prompt;

	int			cmd_count;
	int			pipe_count;
	int			exit_status;
	t_vector	tokens;

}				t_msh;

/* ************************************************************************** */
/*                                 FUNCTIONS                                  */
/* ************************************************************************** */

/* **************** */
/*      100        */
/* *************** */
// FIRST FUNCTIONS -> MAIN AND INITs

//100_main.c
int		main(int ac, char **av, char **envp);
int		msh_loop(t_msh *msh);

//110_init.c
void	init_msh(t_msh *msh, char **envp);
void	init_struct(t_msh *msh);
void	init_vector(t_vector *vector, size_t size);
char	**init_env(char **envp);

//111_init_var.c
char	*get_variable(char *v, char **envp);

//120_prompt.c
char	*build_prompt(t_msh *msh);

/* **************** */
/*      200        */
/* *************** */
// TOKENS

//200_tokenization.c
char	**split_input(const char *input, t_msh *msh);
char	*handle_single_quotes(const char *input, int *i);
char	*handle_double_quotes(const char *input, int *i, t_msh *msh);
int		count_words(const char *input);
int		is_whitespace(char c);
char	*expand_var(const char *input, int *i, t_msh *msh);
char	*copy_word(const char *input, int start, int end);

//210_tkns_type.c
t_tkn	*tokenizer(t_msh *msh, char **av);

//220_vectors.c
void	vector_push(t_vector *vector, t_tkn *token);


/* **************** */
/*      300        */
/* *************** */
// PARSERS

//300_parser.c	
t_tkn	*to_parse(t_msh *msh);

//310_bi_tree.c
t_tkn	*create_bin_tree(t_tkn *tokens);

//320_syntax_check.c
int		syntax_check(t_msh *msh, t_tkn *tokens);
int		syntax_check_redirs(t_msh *msh, t_tkn *tokens);
int		syntax_check_pipes(t_msh *msh, t_tkn *tokens);


/* **************** */
/*      400        */
/* *************** */
// EXECUTE

//400_execute.c
int		to_execute(t_msh *msh, t_tkn *tokens);
int		exec_bi(t_tkn *tokens, t_msh *msh);
int		exec_exe(t_tkn *tokens, t_msh *msh);
void	execute(t_msh *msh, t_tkn *tokens);
char	*find_path(char *cmd, char **envp);

//401_build_args.c
char	**build_args(t_tkn *tokens);

//410_signals.c
void	setup_signals(void);
void	sigint_handler(int sig);

//420_exec_one.c
int		exec_one(t_msh *msh, t_tkn *tokens);

//430_exec_more.c
int		exec_more(t_msh *msh, t_tkn *tokens);

/* **************** */
/*      500        */
/* *************** */
// BUILT-INS

//500_echo.c
int		msh_echo(char **argv);
// int		msh_echo(t_msh *msh, t_tkn *tokens);

//510_cd.c
int		msh_cd(char **argv);

//520_pwd.c
int		msh_pwd(void);

//530_export.c
int		msh_export_no_var(char **envp);
int		msh_export(char ***envp, const char *new_var);

//540_unset.c
int		is_variable_match(const char *env_var, const char *var_name);
int		msh_unset(char **argv, char ***envp);

//550_env.c
int		msh_env(char **envp);

//560_exit.c
int		is_num(const char *str);
int		msh_exit(char **argv, t_msh *msh);

/* **************** */
/*      600        */
/* *************** */
// PIPES

//600_pipes.c
int		exec_pipe(t_msh *msh, t_tkn *tokens);
void	apply_pipe(t_tkn *dir, t_msh *msh, int *fd, int multi_fd);

/* **************** */
/*      700        */
/* *************** */
// REDIRECTS

//700_redirs.c
int	exec_redirs(t_tkn *tokens, t_msh *msh);

/* **************** */
/*      800        */
/* *************** */
// ERROR HANDLERS


/* **************** */
/*      900        */
/* *************** */
// FREEs and CLOSEs

//900_free.c
int		free_array(char **str, int error);
void	free_arg(char **argv);
void	free_msh(t_command *command, t_msh *msh, t_tkn *token);
void	free_vector(t_vector *vector);

//910_close.c
void	ft_close(int *fd);


#endif
