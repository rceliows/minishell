#include "../inc/main.h"

static void	get_env_vars_hash(t_env_hash *env_hash, char **home,
			char **user, char **hostname)
{
	*home = get_env_variable(env_hash, "HOME");
	if (!*home)
		*home = "";
	*user = get_env_variable(env_hash, "USER");
	if (!*user)
		*user = "unknown";
	*hostname = get_env_variable(env_hash, "HOSTNAME");
	if (!*hostname)
		*hostname = "localhost";
}

static void	build_user_host(char *prompt, char *user, char *hostname)
{
	ft_strlcpy(prompt, user, BUFFER_SIZE);
	ft_strlcat(prompt, "@", BUFFER_SIZE);
	ft_strlcat(prompt, hostname, BUFFER_SIZE);
	ft_strlcat(prompt, ":", BUFFER_SIZE);
}

static void	build_directory_path(char *prompt, char *cwd, char *home)
{
	size_t	home_len;

	if (home && *home)
	{
		home_len = ft_strlen(home);
		if (ft_strncmp(cwd, home, home_len) == 0)
		{
			ft_strlcat(prompt, "~", BUFFER_SIZE);
			if (cwd[home_len] && cwd[home_len] == '/')
				ft_strlcat(prompt, cwd + home_len, BUFFER_SIZE);
		}
		else
			ft_strlcat(prompt, cwd, BUFFER_SIZE);
	}
	else
		ft_strlcat(prompt, cwd, BUFFER_SIZE);
}

char	*setup_prompt_hash(char *cwd, t_env_hash *env_hash)
{
	char	*prompt;
	char	*home;
	char	*user;
	char	*hostname;

	prompt = malloc(BUFFER_SIZE);
	if (!prompt)
		return (NULL);
	prompt[0] = '\0';
	get_env_vars_hash(env_hash, &home, &user, &hostname);
	build_user_host(prompt, user, hostname);
	build_directory_path(prompt, cwd, home);
	ft_strlcat(prompt, "$ ", BUFFER_SIZE);
	return (prompt);
}