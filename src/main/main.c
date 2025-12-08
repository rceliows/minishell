#include "../inc/main.h"

volatile sig_atomic_t	g_shell_signal = 0;

static int	main_loop_hash(char *input, t_env_hash *env_hash,
			char *prompt, int *exit_code)
{
	t_token			*tokens;
	t_cmd			*cmd;
	t_interpreter	interpreter;

	if (*input)
		add_history(input);
	tokens = tokenize_line(input, env_hash, exit_code);
	if (! tokens)
		return (-1);
	cmd = parse_commands(tokens);
	if (! cmd)
	{
		free_tokens(tokens);
		*exit_code = 2;
		return (-1);
	}
	interpreter.prompt = prompt;
	interpreter.tokens = tokens;
	interpreter.cmd = cmd;
	execute_pipeline(&interpreter, env_hash, exit_code);
	free_tokens(tokens);
	free_cmds(cmd);
	return (*exit_code);
}

static t_env_hash	*init_shell_env(char **envp, int *exit_code)
{
	t_env_hash	*env_hash;

	*exit_code = 0;
	env_hash = copy_envp(envp);
	if (!env_hash)
	{
		write(STDERR_FILENO, "Error: Failed to initialize environment\n", 40);
		*exit_code = 1;
		return (NULL);
	}
	setup_signals();
	return (env_hash);
}

static int	handle_shell_iteration(t_env_hash *env_hash, int *exit_code)
{
	char	*input;
	char	*prompt;
	char	cwd[1024];

	if (handle_shell_signal(exit_code))
		return (0);
	getcwd(cwd, sizeof(cwd));
	prompt = setup_prompt_hash(cwd, env_hash);
	set_env_variable(env_hash, "PWD", cwd);
	input = readline(prompt);
	if (handle_exit_command(input, exit_code))
	{
		free(input);
		free(prompt);
		return (1);
	}
	main_loop_hash(input, env_hash, prompt, exit_code);
	cleanup_shell_iteration(input, prompt);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	int			exit_code;
	t_env_hash	*env_hash;

	(void)argc;
	(void)argv;
	env_hash = init_shell_env(envp, &exit_code);
	if (!env_hash)
		return (exit_code);
	while (1)
	{
		if (handle_shell_iteration(env_hash, &exit_code))
			break ;
	}
	cleanup_hash_table(env_hash);
	rl_clear_history();
	return (exit_code);
}
