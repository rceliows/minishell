#include "../inc/main.h"

int	is_word_delimiter(char c)
{
	return (ft_isspace((unsigned char)c) || ft_strchr("|<>", c) != NULL);
}

int	is_valid_char(char quote, t_variable *var)
{
	return (quote == '"' && var->s[*var->i] == '$'
		&& var->s[*var->i + 1] && (ft_isalpha(var->s[*var->i + 1])
			|| var->s[*var->i + 1] == '_'
			|| var->s[*var->i + 1] == '?'));
}

char	*read_word(const char *s, size_t *i,
			t_env_hash *env_hash, int exit_code)
{
	size_t		cap;
	size_t		len;
	char		*buf;
	t_variable	var;

	var.s = s;
	var.i = i;
	var.env_hash = env_hash;
	var.exit_code = exit_code;
	cap = 42;
	len = 0;
	buf = malloc(cap + 1);
	if (!buf)
		return (NULL);
	while (s[*i] && !is_word_delimiter(s[*i]))
		buf = compute_expansion(&var, buf, &len, &cap);
	if (! buf)
		return (NULL);
	buf[len] = '\0';
	return (buf);
}
