#ifndef SIGNALS_H
# define SIGNALS_H

/* Signal handler functions */
void	handle_sigint(int sig);
void	handle_child_sigint(int sig);
void	handle_child_sigquit(int sig);

/* Signal setup functions */
void	setup_signals(void);
void	setup_child_signals(void);
void	restore_default_signals(void);
void	setup_parent_pipe_signals(void);

/* Process management with signal handling */
void	wait_for_child_process(pid_t pid, int *exit_code, int *quit_printed);

#endif
