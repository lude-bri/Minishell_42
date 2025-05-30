/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 15:40:26 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/12 12:02:28 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# define _GNU_SOURCE

/**
 * @file minishell.h
 * @brief Main header file for the minishell project.
 *
 * This header includes all the definitions, structures, enumerations, and function
 * prototypes necessary for implementing a basic shell in C, capable of parsing,
 * executing commands, managing environment variables, handling redirections, pipes,
 * signals, and built-in shell functionalities.
 */

/* ************************************************************************** */
/*                                  INCLUDES                                  */
/* ************************************************************************** */

# include <unistd.h>
# include <limits.h>
# include <fcntl.h>
# include <stdio.h>
# include <signal.h>
# include <sys/wait.h>
# include <stdbool.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "../lib/libft/libft_42/libft.h"
# include "../lib/libft/libft_42/fprintf.h"

/* ************************************************************************** */
/*                                  DEFINES                                   */
/* ************************************************************************** */

//Max
//# define PATH_MAX 4096     /**< Maximum allowed path length. */
# define BUF_SIZE 4096     /**< Default buffer size. */

//Semantics
# define NO_ENV -1         /**< Indicator for missing environment */
# define NO_VAR -1         /**< Indicator for missing variable. */
# define NO_TOKEN 0        /**< Indicator for no token found. */
# define NO_PATH 0         /**< Indicator for no path. */
# define NO_CMDS 0         /**< Indicator for no commands. */

/* ************************************************************************** */
/*                                  ENUMS								  	  */
/* ************************************************************************** */

/**
 * @enum e_status
 * @brief Execution status codes.
 */
typedef enum e_status
{
	FAILURE,
	SUCCESS
}	t_status;

/**
 * @enum e_token_group
 * @brief Token types used in parsing the command line.
 */
typedef enum e_token_group
{
	TKN_IN,
	TKN_APPEND,
	TKN_OUT,
	TKN_REDIR_ARG,
	TKN_CMD,
	TKN_NULL,
	TKN_BLANK,
	TKN_PIPE,
	TKN_HEREDOC
}	t_token_group;

/**
 * @enum e_cmd_group
 * @brief Built-in command types.
 */
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

/**
 * @struct s_heredoc
 * @brief Structure for managing heredoc instances.
 */
typedef struct s_heredoc
{
	// bool				exist;
	int					i;
	int					len;
	int					fd_heredoc;
	char				*fd_heredoc_path;
	char				*eof;
	int					count_hd;
	struct s_heredoc	*next;
}						t_heredoc;

/**
 * @struct s_tkn_op
 * @brief Helper struct for tokenizing input. Used in 200_tokenization.c
 */
typedef struct s_tkn_op
{
	int		i;
	int		j;
	int		number_words;
	int		start;
	char	**argv;
}			t_tkn_op;

/**
 * @struct s_tkn
 * @brief Represents a parsed token in the input.
 */
typedef struct s_tkn
{
	t_token_group	type;
	t_cmd_group		cmd_type;
	char			*name;
	char			*hd_path;
	char			**cmdargs; // need to create
	int				len;
	struct s_tkn	*tokens;
	struct s_tkn	*left; //to binary tree
	struct s_tkn	*right; //to binary tree
	struct s_tkn	*next; //linked list -> helps w free
}					t_tkn;

/**
 * @struct s_command
 * @brief Represents a parsed command and its arguments.
 */
typedef struct s_command
{
	int		ac; //argument counter
	char	**av; //argument values
	char	*cmd; //command
}			t_command;

/**
 * @struct s_vector
 * @brief Dynamic array for storing tokens.
 */
typedef struct s_vector
{
	int		count;
	int		size;
	t_tkn	**buffer;
}			t_vector;

/**
 * @struct s_msh
 * @brief Core struct for the entire shell's state.
 */
typedef struct s_msh
{
	t_command	*cmds;
	t_tkn		*tree_head; //binary tree head to execute
	char		**envp;
	char		**ex_envp;
	char		**path;
	char		**arg;

	char		*user;
	char		*home;
	char		*hostname;
	char		*pwd;
	char		*prompt;
	char		*line;
	char		*word;

	int			fd_in;
	int			fd_out;

	int			pid;
	int			cmd_count;
	int			pipe_count;
	int			len;
	int			exit_status;
	bool		flag_redir;
	bool		flag_exit;
	t_vector	tokens;
	t_heredoc	*heredoc;
	size_t		word_size;

}				t_msh;

/**
 * @struct s_exp
 * @brief Struct to handle export logic and states.
 */
typedef struct s_exp
{
	bool	flag;
	size_t	len;
	size_t	prefix_len;
	char	*no_sign;
	char	*equal_sign;
	char	*add_sign;
	char	*remove_sign;
	char	*var;
	char	*var_name;
	char	*value;
	char	*result;
	char	*new_entry;
	char	*updated_value;
	char	*existing_value;
	char	*to_add;
	char	*to_remove;
	char	*found;
	char	**new_envp;
	int		start;
}				t_exp;

/**
 * @struct s_quote
 * @brief Used for managing quotes in strings.
 */
typedef struct s_quote
{
	char	*buffer;
	int		*buf_i;
	int		*i;
}				t_quote;

/**
 * @struct s_params
 * @brief Used to pass context parameters during parsing/tokenizing.
 */
typedef struct s_params
{
	const char	*input;
	t_tkn_op	*sp;
	t_msh		*msh;
	char		*buffer;
	int			*buf_i;
	bool		is_first_word;
}				t_params;

/**
 * @var g_signal
 * @brief Global signal state.
 */
extern volatile int	g_signal;

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
void	increment_shlvl(char **envp);

//120_prompt.c
char	*build_prompt(t_msh *msh);

/* **************** */
/*      200        */
/* *************** */
// TOKENS

//200_tokenization.c
char	*copy_word(const char *input, int start, int end);
int		count_words(const char *input);
void	process_tokens(const char *input, t_tkn_op *sp, t_msh *msh);
void	process_input(const char *input, t_tkn_op *sp, t_msh *msh);
char	**split_input(const char *input, t_msh *msh);

//201_tkns_handlers.c
void	handle_input_redir(const char *input, t_tkn_op *sp, t_msh *msh);
void	handle_output_redir(const char *input, t_tkn_op *sp, t_msh *msh);
void	handle_pipes(const char *input, t_tkn_op *sp);
void	handle_expand(const char *input, t_tkn_op *sp, t_msh *msh);
void	handle_quotes(const char *input, t_tkn_op *sp, t_msh *msh);

//202_tkns_handlers_2.c
char	*handle_double_quotes(const char *input, int *i, t_msh *msh);
char	*handle_single_quotes(const char *input, int *i);
void	handle_double_in_redir(const char *input, t_tkn_op *sp);
void	handle_single_in_redir(const char *input, t_tkn_op *sp, t_msh *msh);

//203_tkns_handlers_3.c
void	handle_double_out_redir(const char *input, t_tkn_op *sp);
void	handle_single_out_redir(const char *input, t_tkn_op *sp, t_msh *msh);

//204_tkns_handler_words.c
void	handle_words(const char *input, t_tkn_op *sp, t_msh *msh);

//205_tkns_misc.c
int		quote_helper(const char *input, int i);
bool	special_char_helper(const char *input, int *i);
bool	regular_char_helper(const char *input, int *i);
void	append_char_to_word(const char *input, int *i, t_msh *msh);
int		take_len(const char *input);

//206_tkns_misc2.c
int		is_whitespace(char c);
void	skip_whitespace(const char **input);

//207_tkns_expander.c
char	*expand_helper(const char *input, int *i, t_msh *msh);
char	*expand_var(const char *input, int *i, t_msh *msh);

//210_tkns_type.c
int		token_len(t_tkn *tokens);
t_tkn	*tkn_new(t_msh *msh, char *content);
void	verify_tkn_cmd(t_tkn *token);
void	assign_tkn(t_tkn *token, char **av, t_msh *msh);
t_tkn	*tokenizer(t_msh *msh, char **av);

//211_tkns_type_handler.c
int		verify_quotes(char operator, char *line);
int		verify_inbetween_quotes(char operator, char *line);
void	handle_heredoc_token(t_tkn *token, char *line);
void	handle_other_tokens(t_tkn *tkn, char **av, t_msh *msh);

//212_tkns_type_handler_2.c
void	handle_blank_and_null(t_tkn *token);
void	handle_pipe_token(t_tkn *token, char *line);
void	handle_append_token(t_tkn *token, char *line);
void	handle_out_token(t_tkn *token, char *line);
void	handle_in_token(t_tkn *token, char *line);

//220_vectors.c
void	vector_push(t_vector *vector, t_tkn *token);

/* **************** */
/*      300        */
/* *************** */
// PARSERS

//300_parser.c
int		count_pipes(t_tkn *tokens);
void	error_syntax(t_msh *msh);
int		is_operator(char *line);
int		syntax(t_msh *msh, char *line);
t_tkn	*to_parse(t_msh *msh);

//301_parser_check.c
int		check_initial_syntax(t_msh *msh, char *line);
int		check_str_syntax(t_msh *msh, char *line);
int		initial_syntax_checks(t_msh *msh, char *line, char *str);
int		detailed_syntax_checks(t_msh *msh, char *str, char *line);

//302_parser_check2.c
int		check_echo_command(char *line);
int		check_operators(t_msh *msh, char *line, int *i);
int		check_line_syntax(t_msh *msh, char *line);

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
void	execute_builtin_commands(t_tkn *tokens, t_msh *msh);
int		exec_bi(t_tkn *tokens, t_msh *msh);
int		exec_exe(t_tkn *tokens, t_msh *msh);
void	exec_special(t_tkn *tokens, t_msh *msh);
void	execute(t_msh *msh, t_tkn *tokens);

//401_build_args.c
char	**build_args(t_tkn *tokens);

//402_execute_misc.c
int		to_execute(t_msh *msh, t_tkn *tokens);
void	handle_redirections(t_tkn *tokens, t_msh *msh, int *fd_in, int *fd_out);
char	*check_direct_path(char *cmd);
char	*search_in_env_path(char *cmd, char **envp);
char	*find_path(char *cmd, char **envp, t_tkn *tokens);

//403_execute_misc2.c
void	handle_child_process(t_tkn *tokens, t_msh *msh);
void	handle_parent_process(t_msh *msh, int status);
void	handle_command_not_found(t_msh *msh, t_tkn *tokens, char **args);
void	handle_execve_error(char *path, char **args, t_msh *msh, t_tkn *tokens);

//410_signals.c
void	setup_signals(void);
void	sigint_handler(int sig);

//420_exec_one.c
int		exec_one(t_msh *msh, t_tkn *tokens);
int		is_bi(t_tkn *tokens);

//421_exec_more_misc.c
void	handle_child_builtin(t_msh *msh, t_tkn *tokens);
void	handle_parent_builtin(int pid, t_msh *msh);

//430_exec_more.c
int		exec_more(t_msh *msh, t_tkn *tokens);

/* **************** */
/*      500        */
/* *************** */
// BUILT-INS

//500_echo.c
int		msh_echo(char **argv, t_msh *msh, t_tkn *tokens);

//510_cd.c
int		msh_cd(char **argv, char **envp);
int		find_env_var(char **envp, const char *var);
void	update_env_var(char **envp, const char *var, const char *value);

//520_pwd.c
int		msh_pwd(t_msh *msh, t_tkn *tokens);

//530_export.c
int		msh_export_no_var(char **envp);
int		parse_variable(const char *new_var, t_exp *exp);
int		handle_variable_update(t_msh *msh, t_exp *exp, const char *new_var);
int		msh_export(t_msh *msh, char ***envp, const char *new_var);

//531_export_change_var.c
int		add_new_variable(char ***envp, t_exp *exp, const char *new_var);
int		handle_addition(t_exp *exp);
int		handle_removal(t_exp *exp);
int		update_variable_entry(char ***envp, t_exp **exp, int i);
int		update_existing_variable(char ***envp, t_exp *exp);

//532_ export_misc.c
void	print_env_var(char *env_var);
int		find_existing_variable(char ***envp, t_exp **exp);
int		variable_exists(char ***envp, const char *var);
int		update_existing_variable_env(char ***envp, t_exp *exp);

//533_export_misc2.c
int		sort_envp(char **envp);
int		is_alpha(const char *str);
int		sanity_check_export(const char *var);

//540_unset.c
int		remove_variable(char ***envp, char *var);
int		is_variable_match(const char *env_var, const char *var_name);
int		msh_unset(char **argv, char ***envp);

//550_env.c
int		msh_env(char **envp, t_msh *msh, t_tkn *tkn);

//560_exit.c
int		msh_exit(char **argv, t_msh *msh, t_tkn *tokens);

//561_exit_misc.c
int		is_num(const char *str);
int		is_letter(const char *str);
int		is_sign(char *argv);

/* **************** */
/*      600        */
/* *************** */
// PIPES

//600_pipes.c
void	apply_pipe(t_tkn *dir, t_msh *msh, int *fd, int multi_fd);
int		setup_pipe_and_fork(t_msh *msh, t_tkn *tokens, int *fd);
int		exec_pipe(t_msh *msh, t_tkn *tokens);

//601_pipes_child.c
void	handle_left_child(t_tkn *tokens, t_msh *msh, int *fd);
void	handle_right_child(t_tkn *tokens, t_msh *msh, int *fd);
void	wait_for_child(t_msh *msh, pid_t pid_left, pid_t pid_right);

/* **************** */
/*      700        */
/* *************** */
// REDIRECTS

//700_redirs.c
void	redirs(t_tkn *tokens, t_msh *msh);
int		exec_redirs(t_tkn *tokens, t_msh *msh);

//701_redirs_2.c
void	process_redirection(t_tkn *tokens, t_msh *msh);
void	update_token_types(t_tkn *tokens);
void	handle_redirs(t_tkn *tokens, t_msh *msh);
void	handle_command_arguments(t_tkn *tokens, t_msh *msh);
void	redirs_2(t_tkn *tokens, t_msh *msh);

//710_redir_in.c
void	redir_in(t_tkn *tokens, t_msh *msh);

//720_redir_out.c
void	redir_out(t_tkn *tokens, t_msh *msh);

//730_redir_app.c
void	redir_append(t_tkn *tokens, t_msh *msh);

//740_heredoc.c
void	set_heredoc_signals(void);
int		fill_heredoc(int temp_fd, char *eof);
void	start_heredoc(t_msh *msh, t_tkn *tokens);
void	heredoc_exec(t_msh *msh, t_tkn *tokens);

//741_heredoc_pipe.c
void	heredoc_child_process(char *eof, char *temp_path, t_msh *msh);

//742_heredoc_utils.c
void	transform(t_tkn *tokens, char *path);
void	assign_heredoc(t_heredoc **heredoc, char *eof);
char	*find_eof(t_tkn *tokens);
int		heredoc_len(t_heredoc *heredoc);
int		find_heredoc(t_tkn *tokens);

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
void	free_heredoc(t_heredoc *heredoc);

//901_free_heredoc.c
void	free_hd(t_heredoc *heredoc);
void	free_vector_2(t_vector *vector, t_msh *msh);
void	free_all_heredoc(t_msh *msh);

//902_free_parse.c
void	free_parse(t_msh *msh);

//910_close.c
void	ft_close(int *fd);
void	free_and_close_all(t_msh *msh, t_tkn *tokens);

#endif
