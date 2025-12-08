#include "../inc/main.h"

int	advance_var(t_expand_context *ctx, int i)
{
	int	j;

	if (ctx->input[i + 1] == '?')
		return (i + 2);
	j = i + 1;
	while (ctx->input[j] && (ft_isalnum(ctx->input[j]) || ctx->input[j] == '_'))
		j++;
	return (j);
}

char	*exit_code_str(int exit_code)
{
	int		n;
	char	*res;

	if (exit_code < 0)
		n = 0;
	else if (exit_code > 255)
		n = exit_code % 256;
	else
		n = exit_code;
	res = ft_itoa(n);
	if (!res)
		return (ft_strdup("0"));
	return (res);
}

static int	get_regular_var_len(t_expand_context *ctx, int start, int end)
{
	char	*var_name;
	char	*val;
	int		len;

	var_name = ft_substr(ctx->input, start, end - start);
	len = 0;
	if (var_name)
	{
		val = get_env_variable(ctx->env_hash, var_name);
		if (val)
			len = ft_strlen(val);
		free(var_name);
	}
	return (len);
}

int	calc_var_len(t_expand_context *ctx, int pos, int *new_pos)
{
	int		start;
	int		end;

	if (ctx->input[pos + 1] == '?')
	{
		*new_pos = pos + 2;
		return (count_digits(ctx->exit_code));
	}
	if (!extract_var_bounds(ctx->input, pos, &start, &end))
	{
		*new_pos = pos + 1;
		return (1);
	}
	*new_pos = end;
	return (get_regular_var_len(ctx, start, end));
}

int	calc_total_len(t_expand_context *ctx)
{
	int	i;
	int	len;
	int	new_pos;

	i = 0;
	len = 0;
	while (ctx->input[i])
	{
		update_quote_state(ctx->input[i], ctx);
		if (ctx->input[i] == '$' && ctx->input[i + 1]
			&& (ft_isalpha(ctx->input[i + 1]) || ctx->input[i + 1] == '_'
				|| ctx->input[i + 1] == '?')
			&& !ctx->in_single_quotes)
		{
			len += calc_var_len(ctx, i, &new_pos);
			i = new_pos;
		}
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}
