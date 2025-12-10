#include "../inc/main.h"

static void	handle_fd(int fd, t_redir *r, int std_fd, t_cleanup_data *cleanup)
{
	error_check(fd, r, std_fd, cleanup);
	close(fd);
}

static void	compute_redir(t_interpreter *it, t_redir *r,
				t_cleanup_data *cleanup)
{
	if (r->kind == R_IN)
		handle_fd(open(r->arg, O_RDONLY), r, STDIN_FILENO, cleanup);
	else if (r->kind == R_OUT)
		handle_fd(open(r->arg, O_WRONLY | O_CREAT | O_TRUNC, 0644),
			r, STDOUT_FILENO, cleanup);
	else if (r->kind == R_APP)
		handle_fd(open(r->arg, O_WRONLY | O_CREAT | O_APPEND, 0644),
			r, STDOUT_FILENO, cleanup);
	else if (r->kind == R_HEREDOC)
		handle_fd(heredoc_read(it, cleanup, 0), r, STDIN_FILENO, cleanup);
	else if (r->kind == R_HEREDOC_EXP)
		handle_fd(heredoc_read(it, cleanup, 1), r, STDIN_FILENO, cleanup);
}

void	apply_redirections(t_interpreter *it, t_cleanup_data *cleanup)
{
	t_redir	*r;

	r = it->cmd->redir;
	while (r)
	{
		compute_redir(it, r, cleanup);
		r = r->next;
	}
}