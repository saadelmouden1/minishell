/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-moud <sel-moud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 19:46:34 by sel-moud          #+#    #+#             */
/*   Updated: 2024/10/15 19:51:14 by sel-moud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	simple_child_in_builtin(char **cmd, int *fd, t_env *env, int *piped)
{
	pid_t	pid;
	int		pfd[2];
	int		out_fd[2];
	int		status;

	(pipe(pfd), pid = fork());
	if (!pid)
	{
		if (piped[0] && piped[0] <= piped[5])
			dup2(fd[0], 0);
		if (piped[0] > 1)
			dup2(pfd[1], 1);
		else
			close(fd[0]);
		close_pipe_ends(pfd[0], pfd[1]);
		dup2(1, out_fd[1]);
		status = builtin_cmd_exec_in_child(cmd, env, out_fd, piped);
		exit(WEXITSTATUS(status));
	}
	close_pipe_ends(pfd[1], fd[0]);
	if (piped[0] > 1)
		fd[0] = pfd[0];
	else
		close(pfd[0]);
	return (1);
}

void	builtin_exec_and_exit(char **cmd, t_env *env, int *out_fd, int *piped)
{
	int	status;

	status = builtin_cmd_exec_in_child(cmd, env, out_fd, piped);
	exit(WEXITSTATUS(status));
}

int	child_redirection(char **_cmd_, int *_fd, t_env *env, int *_piped)
{
	pid_t	pid;
	int		_out_fd[2];

	_out_fd[1] = 1;
	if (_piped[8] && _piped[7])
		_out_fd[1] = _piped[2];
	if (_piped[0] > 1 && (!_piped[8] || !_piped[7]))
		pipe(_out_fd);
	pid = fork();
	if (!pid)
		builtin_exec_and_exit(_cmd_, env, _out_fd, _piped);
	if (_piped[8] && _piped[7])
	{
		close(_out_fd[1]);
		_piped[7] = 0;
	}
	if (!_piped[6] && !_piped[7])
		_piped[8] = 0;
	if (_piped[0] > 1 && (!_piped[8] || !_piped[7]))
	{
		close(_out_fd[1]);
		_fd[0] = _out_fd[0];
	}
	return (1);
}

int	is_string_numeric(char *s_1)
{
	int	a;

	a = 0;
	while (s_1[a])
	{
		if (!ft_isdigit(s_1[a]))
			return (0);
		a++;
	}
	return (1);
}

int	manage_bultin_exec(char **cmd, int *fd, t_env *env, int *piped)
{
	int	status;

	status = 0;
	piped[10] += 1;
	if (piped[0])
	{
		if (!piped[8])
			status = simple_child_in_builtin(cmd, fd, env, piped);
		else
			status = child_redirection(cmd, fd, env, piped);
		free_str_arr(cmd);
	}
	else
		status = simple_builtin_execution_managment(cmd, fd, env, piped);
	return (status);
}
