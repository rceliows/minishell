#ifndef COMMANDS_H
# define COMMANDS_H

/* Main command execution logic */
int		execute_shell_command_with_pipes_hash(char *input,
			t_env_hash *env_hash, int *exit_code);
int		execute_parsed_command_hash(char *input,
			t_env_hash *env_hash, int *exit_code);

/* Simple command execution */
int		execute_shell_command_hash(char *input,
			t_env_hash *env_hash, int *exit_code);

/* External command execution */
void	execute_simple_external_hash(char **cmd_args,
			t_env_hash *env_hash, int *exit_code);
int		fork_and_execute(char **cmd_args, char *cmd_path, char **envp);
int		execute_external_command(char **cmd_args, char **envp);

/* Input/Output redirection handling */
void	execute_simple_command_with_redirection_hash(t_parse_result *parsed,
			t_env_hash *env_hash, int *exit_code);
void	handle_builtin_with_redirection_hash(t_parse_result *parsed,
			char **cmd_args, t_env_hash *env_hash, int *exit_code);
void	handle_external_with_redirection_hash(t_parse_result *parsed,
			char **cmd_args, t_env_hash *env_hash, int *exit_code);
void	execute_external_child_hash(t_parse_result *parsed,
			char **cmd_args, t_env_hash *env_hash);

/* Redirection utilities */
void	setup_input_redirection(t_parse_result *parsed, int *input_fd);
void	setup_output_redirection_hash(t_parse_result *parsed, int *output_fd);
void	restore_standard_fds(int orig_stdin, int orig_stdout,
			int input_fd, int output_fd);
int		get_output_flags(t_parse_result *parsed);

/* Shell script execution utilitie*/
int		run_as_shell_script(char *path, char **args, char **env);
int		heredoc_read(t_interpreter *it, t_cleanup_data *cleanup, int expand);

#endif
