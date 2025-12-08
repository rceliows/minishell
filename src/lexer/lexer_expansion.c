#include "../inc/main.h"

static char	*parse_dollar(const char *s, size_t start,
				size_t *i, t_env_hash *env_hash)
{
	size_t	var_start;
	size_t	var_end;
	char	*var_name;
	char	*var_value;
	char	*result;

	var_start = *i;
	while (s[*i] && (ft_isalnum(s[*i]) || s[*i] == '_'))
		(*i)++;
	var_end = *i;
	if (var_start == var_end)
	{
		*i = start + 1;
		return (ft_strdup("$"));
	}
	var_name = ft_substr(s, var_start, var_end - var_start);
	if (!var_name)
		return (ft_strdup(""));
	var_value = get_env_variable(env_hash, var_name);
	free(var_name);
	if (var_value)
		result = ft_strdup(var_value);
	else
		result = ft_strdup("");
	return (result);
}

char	*expand_single_variable(const char *s, size_t *i,
					t_env_hash *env_hash, int exit_code)
{
	size_t	start;

	start = *i;
	(*i)++;
	if (s[*i] == '?')
	{
		(*i)++;
		return (exit_code_str(exit_code));
	}
	return (parse_dollar(s, start, i, env_hash));
}

static	char	*compute_quoted(t_variable *var, char **buf,
				size_t *len, size_t *cap)
{
	if (!handle_quoted_content(var, buf, len, cap))
	{
		free(*buf);
		return (NULL);
	}
	return (*buf);
}

static	char	*compute_variable_expansion(t_variable *var, char **buf,
				size_t *len, size_t *cap)
{
	if (!handle_variable_expansion(var, buf, len, cap))
	{
		free(buf);
		return (NULL);
	}
	return (*buf);
}

char	*compute_expansion(t_variable *var, char *buf,
				size_t *len, size_t *cap)
{
	char	*s;
	size_t	*i;

	s = (char *)var->s;
	i = var->i;
	if (s[*i] == '\'' || s[*i] == '"')
		buf = compute_quoted(var, &buf, len, cap);
	else if (s[*i] == '$' && s[*i + 1]
		&& (ft_isalpha(s[*i + 1]) || s[*i + 1] == '_' || s[*i + 1] == '?'))
		buf = compute_variable_expansion(var, &buf, len, cap);
	else
	{
		if (!append_char_to_buffer(&buf, len, cap, s[*i]))
		{
			free(buf);
			return (NULL);
		}
		(*i)++;
	}
	return (buf);
}
