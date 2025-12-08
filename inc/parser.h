#ifndef PARSER_H
# define PARSER_H

/* Parse result structure */
typedef struct s_parse_result
{
	char	*infile_name;
	char	*outfile_name;
	char	*heredoc_delimiter;
	char	**cmds;
	int		is_append;
	int		is_here_doc;
	int		num_commands;
}	t_parse_result;

/* Main input parsing */
t_parse_result	*extract_input(char *input);
int				process_input_tokens(char *input, t_parse_result *parsed,
					char **temp_commands);
int				allocate_commands_array(t_parse_result *parsed,
					char **temp_commands, int cmd_count);
int				setup_temp_commands(char ***temp_commands);
int				finalize_parsed_result(t_parse_result *parsed,
					char **temp_commands, int cmd_count);

/* Token processing */
int				process_single_token(char *input, int *pos,
					t_parse_result *parsed, char **temp_commands);
int				handle_redirection_tokens(char *input, int *pos,
					t_parse_result *parsed);
void			handle_input_redirection(char *input, int *pos,
					t_parse_result *parsed);
void			handle_output_redirection(char *input, int *pos,
					t_parse_result *parsed);
int				should_break_cmd(char *input, int pos, int in_quotes);

/* Command string extraction */
char			*extract_command_string(char *input, int *pos);

/* Word extraction from input */
char			*extract_word_argv(char *input, int *pos);

/* Shell command parsing */
char			**shell_parser(char *input);

/* Shell parser utilities */
char			*allocate_and_copy(char *argv, int start, int len);
int				extract_quoted_content(char *argv, int *i, int *start);
int				extract_regular_content(char *argv, int *i, int *start);
char			**handle_empty_input(void);
int				skip_whitespace(char *input, int *i);
char			**allocate_result_array(int size);

/* Command path resolution */
char			*command_path(char *cmd, char **envp);
char			*search_in_env_path(char *cmd, char **envp);
char			*search_in_default_paths(char *cmd);
char			*search_in_paths(char **paths, char *cmd);
char			*join_path_and_cmd(char *dir, char *cmd);

/* Setup for pipex execution */
char			**transform_into_argv(t_parse_result *parsed);
int				set_program_name(char **result, t_parse_result *parsed, int *i);
int				set_input_args(char **result, t_parse_result *parsed, int *i);
int				set_output_arg(char **result, t_parse_result *parsed, int *i);
int				add_commands(char **result,
					t_parse_result *parsed, int *i, int argc);

/* Parser cleanup functions */
void			cleanup_results(t_parse_result *parsed);
void			init_result_fields(t_parse_result *parsed);
void			skip_leading_whitespace(char *input, int *pos);
t_cmd			*parse_error_cleanup(t_cmd *head);

/* Parser commands*/
t_cmd			*parse_commands(t_token *tokens);

/* Redirection utilities */
t_redir			*redir_new(t_redir_kind k, const char *arg);
void			redir_push(t_redir **head, t_redir *node);
t_redir_kind	get_redir_kind(t_token_type type);
int				handle_redir_token(t_cmd *cur, t_token *tokens);

#endif
