/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-azz <isel-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 00:21:42 by isel-azz          #+#    #+#             */
/*   Updated: 2024/10/16 00:21:43 by isel-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "./libft/libft.h"
# include <errno.h>
# include <stdio.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <unistd.h>

int						g_var_tn;

# define READ_FILE 10
# define READ_FROM_APPEND 15
# define WRITE_FILE 20
# define WRITE_FILE_APPEND 30
# define EXECUTE_FILE 40
# define FILE_READY 50

typedef enum e_typetoken
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HEREDOC
}						t_typetoken;

typedef struct s_token
{
	t_typetoken			token_type;
	char				*value;
	struct s_token		*next;
}						t_token;

typedef struct s_ast_node
{
	t_typetoken			type;
	int					file_type;
	char				**args;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}						t_ast_node;

typedef struct s_env
{
	char				**original_env;
	char				***parsed_env;
}						t_env;

// Input Validation I-V
int						check_syntax_error(char *input);
int						unclosed_quote(char *input);
int						invalid_redirection(char *input);
int						invalid_pipe(char *input);
int						logical_operators(char *input);
// Input-Validation utils
char					*skip_spaces(char *input);
void					quote_count(char c, int *sq_count, int *dq_count);
int						invalid_operator(char **input);
// Tokenization T
t_token					*tokenize_input(char *input);
void					found_special_char(char **input, t_token **tokens);
void					found_word(char **input, t_token **tokens);
// Tokenization utils
t_token					*generate_token(t_typetoken type, char *token_value);
void					add_token(t_token **tokens, t_token *new_token);
void					add_word_token(char **start, char **input,
							t_token **tokens);
void					free_tokens(t_token *tokens);
void					quote_status(char c, int *quoted, char *quote);
// Parsing
t_ast_node				*parse(t_token **tokens);
t_ast_node				*parse_pipe(t_token **tokens);
t_ast_node				*parse_redirection(t_token **tokens);
t_ast_node				*create_file_node(t_token *token);
t_ast_node				*parse_command(t_token **tokens);
// Parsing utils
t_ast_node				*new_ast_node(t_typetoken type);
void					free_ast(t_ast_node *node);
int						count_args(t_token *current);
t_ast_node				*create_redir_node(t_token **tokens,
							t_token *placeholder);
void					fill_args(t_ast_node *node, t_token **tokens,
							int args_count);
// Str utils
int						str_size(char *str, char end);
int						array_str_count(char **arr);
char					*rm_quotes(char *str, int s_q_c, int d_q_c, int a);
char					*strcopy(char *src);
void					string_copy(char *s1, char *s2, int st, int en);
int						str_to_int(char *str);
void					free_str_arr(char **arr);
int						ft_strcmp(char *s_1, char *s_2, char *s_3);

// Env
int						shell_initializing_with_env(t_env *env,
							char **original_env);
void					initialize_default_variables(t_env *env, int a);
int						env_structer_shell_initializing(t_env *env,
							char **original_env, int a);
char					**duplicate_env_varibles(char **env);
int						is_valid_variable_start(char *str, int index, int con);
void					append_env_var(char *var, t_env *env);
int						find_env_var_index(t_env *env, char *name);
void					add_env_entry(t_env *env, char *cmd, int b, int con);
void					update_env_value_at_index(t_env *env, char *cmd, int a,
							int con);
void					remove_env_entry(t_env *env, int c);
char					***duplicate_env_structure(t_env *env, int a, int a_2,
							int d);
char					**copy_env_exclude_index(char **env, int a, int a_2,
							int d);
void					replace_env_var(char *var, t_env *env);
void					update_env_status(t_env *env, int status, char *start);
char					*str_without_char(char *str, char del);
void					expand_variables_in_ast(t_ast_node *head, t_env *env);
char					**refactore_args_array(char **args,
							int *quick_norm_fix);
char					*recursively_expand_variables(char *var, t_env *env,
							int __con, int *f_arr);
char					*expand_variable_in_string(char *var, t_env *env, int a,
							int *f_hole);
char					*replace_variable_with_value(char *old_var, char *__new,
							int st, int end);
int						detected_flaws(char **array);
int						is_flawed_str(char *str, int a, int b, int res);
int						sus_getit_right(char *str, int s_q, int d_q);
int						simplified_refactor_thing(char **array, int index,
							char *str, int in);
// Utils
int						count_digits(int number);
void					free_env_var(char ***arr);
void					cleanup_exit(t_env *env, int stat);
int						is_space(char *line);
int						check_line(char **line);
char					*str_no_char(char *str, char del);
// SIGNALS
void					setup_signal_handlers(void);
void					handle_ctrl_c(int a);
void					child_ctrl_c(int sig_num);
// EXECUTION
int						echo_cmd(char **_cmd, int *_out_fd);
int						env_or_pwd_cmd(char *cmd, t_env *env, int c,
							int *out_fd);
char					**export_cmd(char **cmd, t_env *env, int *out_fd,
							int **s);
char					**unset_or_export_cmd(char **cmd, t_env *env,
							int *out_fd, int *s);
int						cd_cmd(char **cmd, t_env *env, int *out_fd);
void					set_new_pwd_in_env(char *new, t_env *env, int c);
int						change_current_directory(char *path, t_env *env);
char					*get_current_working_directory(int size, int tries,
							int fd);
char					***sort_tha_array(char ***_array, int _si);
void					print_export_declaration_to_fd(t_env *env, int *out_fd);
void					print_env_var_to_fd(char *str_1, char *str_2, int fd);
void					print_export_vars(char ***arr, int a, int fd);
int						string_weight_compare(char *s_1, char *s_2);
int						check_array_arrangment(char ***array, int _si);
int						is_valid_echo_paran(char *s);
void					__exit(char **_cmd_);
int						export_print_or_export(char **cmd);
int						export_statment_check(char *cmd);
int						simple_child_in_builtin(char **cmd, int *fd, t_env *env,
							int *piped);
void					builtin_exec_and_exit(char **cmd, t_env *env,
							int *out_fd, int *piped);
int						child_redirection(char **_cmd_, int *_fd, t_env *env,
							int *_piped);
int						manage_bultin_exec(char **cmd, int *fd, t_env *env,
							int *piped);
int						execute_builtin_with_piping(char **_cmd_, int *_fd,
							t_env *env, int *_piped);
int						builtin_with_simple_piping_exec(char **cmd, int *fd,
							t_env *env, int *piped);
int						simple_builtin_execution_managment(char **cmd, int *fd,
							t_env *env, int *piped);
void					prepare_ast_nodes_to_execute(t_ast_node *head);
void					count_red_and_pipes(t_ast_node *head, int *piped);
void					init_or_reset_pipe_state(int *piped, int f);
int						switch_fds_id(int *_piped, int index, int index_2,
							int con);
int						file_to_redirection_open(t_ast_node *head, int *_piped,
							t_env *env, int status);
int						check_builtin_cmd(char *cmd);
int						builtin_cmd_exec_in_child(char **cmd, t_env *env,
							int *out_fd, int *piped);
int						execute_basic_command(char **_cmd_, int *_fd,
							char **env, int *_piped);
int						execute_command_with_redirection(char **_cmd_, int *_fd,
							char **env, int *_piped);
int						preparation_to_exexcute_command(char **cmd, int *fd,
							int *piped, t_env *env);
int						wait_for_children(int status, int *_piped);
int						manage_piped_execution(t_ast_node *head, int *piped,
							t_env *env, int *fd);
int						handle_redirections_cmd(t_ast_node *head, int *piped,
							t_env *env, int *fd);
int						ast_node_execution(t_ast_node *head, int *piped,
							t_env *env);
void					manage_execution_commands(t_ast_node *head, t_env *env,
							int *status);
int						str_comper(char *s1, char *s2, int max);
int						is_string_numeric(char *s_1);
void					exite_herdoc(int a);
int						is_there_quotes(char *s);
char					*create_subpath_from_var(char *env_v, char *f,
							int *indx);
char					*verify_path_without_env(char *f, int mode);
char					*fetch_file_path(char *file, char **envp, char *env_var,
							int mode);
char					*find_next_substring(char *str, char del, int *index);
int						find_substr_index(char **haystack, char *needle, int n);
int						is_path_accessible(char *path, int mode);
void					increment_path_index(char *env_v, int *indx, int *a);
int						count_substrings(char *s, char del);
void					child_fds_managment(int *_piped, int *_fd, int *fd_);
void					parent_fds_managment(int *_piped, int *_fd, int *fd_);
void					close_pipe_ends(int fd1, int fd2);
int						here_doc_execution(char *delmtr, int *piped,
							t_env *env);
char					**prepare_cmd_arguments(char *cmd, char **envp, int c);
char					**merge_command_args(char **f_args, char **_cmd_);
void					read_and_write(int std_out, char *limiter, t_env *env,
							int is_expandable);
int						get_shell_exit_status(int err);
void					sus_dir_chek(char *path, char *f, int *status);
int						specify_what_error_stuff(char *file, int status);
int						verify_cmdfile_peresion(t_ast_node *head, char **env);

#endif
