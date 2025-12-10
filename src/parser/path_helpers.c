#include "../inc/main.h"

static char	*build_path_helper(char *full_path, char **paths,
				char *cmd, size_t i)
{
	size_t	j;
	char	*tmp;

	j = 0;
	tmp = full_path;
	while (paths[i][j])
	{
		*tmp++ = paths[i][j];
		j++;
	}
	*tmp++ = '/';
	j = 0;
	while (cmd[j])
	{
		*tmp++ = cmd[j];
		j++;
	}
	*tmp = '\0';
	return (full_path);
}

static char	*build_path(char *full_path, char **paths, char *cmd)
{
	size_t	i;

	i = 0;
	while (paths[i])
	{
		full_path = malloc(strlen(paths[i]) + strlen(cmd) + 2);
		if (!full_path)
		{
			free_split(paths);
			return (NULL);
		}
		full_path = build_path_helper(full_path, paths, cmd, i);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

char	*get_command_path(char *cmd, t_env_hash *env_hash)
{
	char	*path_env;
	char	**paths;
	char	*full_path;

	if (access(cmd, X_OK) == 0)
		return (strdup(cmd));
	path_env = get_env_variable(env_hash, "PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	full_path = build_path(NULL, paths, cmd);
	free_split(paths);
	return (full_path);
}