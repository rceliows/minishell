#include "../inc/main.h"

char	*safe_strjoin_free(char *s1, const char *s2)
{
	char	*joined;

	joined = ft_strjoin(s1, s2);
	free(s1);
	return (joined);
}

int	cleanup_and_exit(char *prompt, int exit_code)
{
	if (prompt)
		free(prompt);
	rl_clear_history();
	return (exit_code);
}

size_t	ft_arrlen(char **s)
{
	size_t	i;

	if (!s)
		return (0);
	i = 0;
	while (s[i])
		i++;
	return (i);
}
