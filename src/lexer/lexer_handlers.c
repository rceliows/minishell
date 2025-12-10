#include "../inc/main.h"

static char	*update_buffer_size(char *buf, size_t *cap, size_t required_len)
{
	char	*new_buf;
	size_t	old_cap;

	while (required_len >= *cap)
	{
		old_cap = *cap;
		*cap = *cap * 2;
		new_buf = ft_realloc(buf, old_cap, *cap);
		if (!new_buf)
		{
			free(buf);
			return (NULL);
		}
		buf = new_buf;
	}
	return (buf);
}

static int	append_string_to_buffer(char **buf, size_t *len,
					size_t *cap, const char *str)
{
	size_t	str_len;
	char	*new_buf;

	if (!str)
		return (1);
	str_len = ft_strlen(str);
	new_buf = update_buffer_size(*buf, cap, *len + str_len + 1);
	if (!new_buf)
	{
		*buf = NULL;
		return (0);
	}
	*buf = new_buf;
	ft_strlcpy(*buf + *len, str, str_len + 1);
	*len += str_len;
	return (1);
}

int	append_char_to_buffer(char **buf, size_t *len, size_t *cap, char c)
{
	char	*new_buf;

	new_buf = update_buffer_size(*buf, cap, *len + 2);
	if (!new_buf)
	{
		*buf = NULL;
		return (0);
	}
	*buf = new_buf;
	(*buf)[*len] = c;
	(*len)++;
	return (1);
}

int	handle_variable_expansion(t_variable *var, char **buf,
					size_t *len, size_t *cap)
{
	char	*expanded;

	expanded = expand_single_variable(var->s, var->i,
				var->env_hash, var->exit_code);
	if (expanded)
	{
		if (!append_string_to_buffer(buf, len, cap, expanded))
		{
			free(expanded);
			return (0);
		}
		free(expanded);
	}
	return (1);
}

int	handle_quoted_content(t_variable *var, char **buf,
					size_t *len, size_t *cap)
{
	char	quote;

	quote = var->s[(*var->i)];
	(*var->i)++;
	while (var->s[*var->i] && var->s[*var->i] != quote)
	{
		if (is_valid_char(quote, var))
		{
			if (!handle_variable_expansion(var, buf, len, cap))
				return (0);
		}
		else
		{
			if (!append_char_to_buffer(buf, len, cap, var->s[*var->i]))
				return (0);
			(*var->i)++;
		}
	}
	if (var->s[*var->i] == quote)
	{
		(*var->i)++;
		return (1);
	}
	return (0);
}