#include "../inc/main.h"

static void	process_var_value(char *val, char *out, size_t *out_pos)
{
	size_t	val_len;

	if (val)
	{
		val_len = ft_strlen(val);
		ft_strcpy(out + *out_pos, val);
		*out_pos += val_len;
	}
}

static void	expand_var(t_expand_context *ctx, size_t pos, char *out, size_t *out_pos)
{
	size_t	start;
	size_t	end;
	char	*var_name;
	char	*val;

	if (ctx->input[pos + 1] == '?')
	{
		val = exit_code_str(ctx->exit_code);
		process_var_value(val, out, out_pos);
		free(val);
		return ;
	}
	if (!extract_var_bounds(ctx->input, pos, &start, &end))
	{
		out[(*out_pos)++] = '$';
		return ;
	}
	var_name = ft_substr(ctx->input, start, end - start);
	if (var_name)
	{
		val = get_env_variable(ctx->env_hash, var_name);
		process_var_value(val, out, out_pos);
		free(var_name);
	}
}

static void	perform_expansion(t_expand_context *ctx, char *out)
{
	size_t	i;
	size_t	out_pos;

	i = 0;
	out_pos = 0;
	ctx->in_single_quotes = 0;
	ctx->in_double_quotes = 0;
	while (ctx->input[i])
	{
		update_quote_state(ctx->input[i], ctx);
		if (ctx->input[i] == '$' && ctx->input[i + 1]
			&& (ft_isalpha(ctx->input[i + 1]) || ctx->input[i + 1] == '_'
				|| ctx->input[i + 1] == '?')
			&& !ctx->in_single_quotes)
		{
			expand_var(ctx, i, out, &out_pos);
			i = advance_var(ctx, i);
		}
		else
			out[out_pos++] = ctx->input[i++];
	}
	out[out_pos] = '\0';
}

char	*expand_environment_variables_hash(char *input,
		t_env_hash *env_hash, int exit_code)
{
	t_expand_context	ctx;
	char				*result;
	size_t				len;

	if (!input)
		return (NULL);
	ctx.input = input;
	ctx.env_hash = env_hash;
	ctx.exit_code = exit_code;
	ctx.in_single_quotes = 0;
	ctx.in_double_quotes = 0;
	len = calc_total_len(&ctx);
	result = malloc(len + 1);
	if (!result)
		return (NULL);
	perform_expansion(&ctx, result);
	return (result);
}

char	*expand_environment_variables_with_exit_code(char *input,
		char **envp, int exit_code)
{
	t_env_hash	*temp;

	if (!input || !envp)
		return (NULL);
	temp = create_temp_hash_for_expansion(envp);
	if (!temp)
		return (NULL);
	input = expand_environment_variables_hash(input, temp, exit_code);
	cleanup_hash_table(temp);
	return (input);
}