/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_exec2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-moud <sel-moud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 19:46:39 by sel-moud          #+#    #+#             */
/*   Updated: 2024/10/15 19:49:43 by sel-moud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_builtin_with_piping(char **_cmd_, int *_fd, t_env *env, int *_piped)
{
	int	status;
	int	_out_fd[2];

	_out_fd[1] = 1;
	if (_piped[0] > 1)
		pipe(_out_fd);
	status = builtin_cmd_exec_in_child(_cmd_, env, _out_fd, _piped);
	if (_piped[0] > 1)
	{
		close(_out_fd[1]);
		_fd[0] = _out_fd[0];
	}
	return (status);
}

int	builtin_with_simple_piping_exec(char **_cmd_, int *_fd, t_env *env,
		int *_piped)
{
	int	status;
	int	_out_fd[2];

	_out_fd[1] = 1;
	if (_piped[8] && _piped[7])
		_out_fd[1] = _piped[2];
	if (_piped[0] > 1 && (!_piped[8] || !_piped[7]))
		pipe(_out_fd);
	status = builtin_cmd_exec_in_child(_cmd_, env, _out_fd, _piped);
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
	return (status);
}

int	simple_builtin_execution_managment(char **cmd, int *fd, t_env *env,
		int *piped)
{
	int	status;
	int	ex_status;

	status = 0;
	if (ft_strcmp(cmd[0], "exit", NULL))
	{
		ex_status = 0;
		if (cmd[1] && cmd[2])
			return (1);
		if (cmd[1] && !is_string_numeric(cmd[1]))
			ex_status = 255;
		else if (cmd[1])
			ex_status = str_to_int(cmd[1]);
		free_str_arr(cmd);
		ft_putendl_fd("EXIT", 1);
		cleanup_exit(env, ex_status);
	}
	else if (!piped[8])
		status = execute_builtin_with_piping(cmd, fd, env, piped);
	else
		status = builtin_with_simple_piping_exec(cmd, fd, env, piped);
	return (status);
}
