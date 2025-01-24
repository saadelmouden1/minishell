/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-moud <sel-moud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 19:46:27 by sel-moud          #+#    #+#             */
/*   Updated: 2024/10/15 19:48:51 by sel-moud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	__exit(char **_cmd_)
{
	int	status;

	status = 0;
	if (_cmd_[1] && _cmd_[2])
		status = 1;
	else if (_cmd_[1] && !is_string_numeric(_cmd_[1]))
		status = 255;
	else if (_cmd_[1])
		status = str_to_int(_cmd_[1]);
	free_str_arr(_cmd_);
	exit(status);
}

int	export_print_or_export(char **cmd)
{
	int	a;

	a = 1;
	while (cmd[0] && cmd[a])
	{
		if (cmd[a])
			return (1);
		a++;
	}
	return (0);
}

int	export_statment_check(char *cmd)
{
	int	a;
	int	b;

	a = str_size(cmd, '=');
	if (a > 1 && cmd[a - 1] == '+')
		a -= 1;
	if (a)
	{
		b = 0;
		while (b < a)
		{
			if (!b && !ft_isalpha(cmd[b]) && cmd[b] != '-')
				return (-1);
			else if (b && !ft_isalnum(cmd[b]) && cmd[b] != '-')
				return (-1);
			b++;
		}
	}
	return (a);
}

void	close_pipe_ends(int fd1, int fd2)
{
	close(fd1);
	close(fd2);
}
