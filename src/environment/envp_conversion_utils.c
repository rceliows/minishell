#include "../inc/main.h"

static char	*allocate_and_copy_name(char *s, size_t len)
{
	char	*temp;
	char	*name;

	temp = malloc(len + 1);
	if (!temp)
		return (NULL);
	ft_strlcpy(temp, s, len + 1);
	name = ft_strdup(temp);
	free(temp);
	return (name);
}

static char	*get_value_part(char *s, size_t start_value)
{
	if (s[start_value])
		return (ft_strdup(s + start_value));
	return (ft_strdup(""));
}

static int	extract_name_and_value(char *s, char **name, char **value)
{
	size_t	i;
	size_t	start_value;

	i = 0;
	start_value = 0;
	while (s[i] && s[i] != '=')
		i++;
	if (s[i] == '=')
	{
		*name = allocate_and_copy_name(s, i);
		start_value = i + 1;
		*value = get_value_part(s, start_value);
	}
	else
	{
		*name = ft_strdup(s);
		*value = ft_strdup("");
	}
	if (!*name || !*value)
		return (0);
	return (1);
}

t_env_entry	*extract_envp_value(char *s)
{
	t_env_entry	*current_envp;
	char		*name;
	char		*value;

	if (!s)
		return (NULL);
	current_envp = malloc(sizeof(t_env_entry));
	if (!current_envp)
		return (NULL);
	current_envp->name = NULL;
	current_envp->value = NULL;
	current_envp->next = NULL;
	if (!extract_name_and_value(s, &name, &value))
	{
		free(current_envp);
		return (NULL);
	}
	current_envp->name = name;
	current_envp->value = value;
	if (!current_envp->name || !current_envp->value)
	{
		cleanup_entry_on_error(current_envp);
		return (NULL);
	}
	return (current_envp);
}