/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-moud <sel-moud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 19:46:23 by sel-moud          #+#    #+#             */
/*   Updated: 2024/10/15 19:48:45 by sel-moud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env_var_to_fd(char *str_1, char *str_2, int fd)
{
	if (!ft_strcmp(str_1, "?", NULL) && !ft_strcmp(str_2, "F1", NULL))
	{
		ft_putstr_fd(str_1, fd);
		ft_putstr_fd("=", fd);
		if (str_2[0])
			ft_putstr_fd(str_2, fd);
		write(fd, "\n", 1);
	}
}

void	print_export_vars(char ***arr, int a, int fd)
{
	while (a--)
	{
		if (!ft_strcmp(arr[a][0], "?", NULL))
		{
			ft_putstr_fd("declare -x ", fd);
			ft_putstr_fd(arr[a][0], fd);
			if (!ft_strcmp(arr[a][1], "F1", NULL))
			{
				write(fd, "=", 1);
				ft_putchar_fd('"', fd);
				if (arr[a][1][0])
					ft_putstr_fd(arr[a][1], fd);
				ft_putchar_fd('"', fd);
			}
			write(fd, "\n", 1);
		}
	}
}

int	string_weight_compare(char *s_1, char *s_2)
{
	int	a;

	a = 0;
	while (s_1[a] && s_2[a])
	{
		if (s_1[a] > s_2[a])
			return (0);
		else if (s_2[a] > s_1[a])
			return (1);
		a++;
	}
	if (s_2[a])
		return (1);
	else if (s_1[a])
		return (0);
	return (0);
}

int	check_array_arrangment(char ***array, int _si)
{
	int	a;
	int	b;
	int	c;

	a = 0;
	while (a < _si - 1)
	{
		b = a + 1;
		while (b < _si)
		{
			c = string_weight_compare(array[a][0], array[b][0]);
			if (c || (c < 0 && (b - a) > 1))
				return (0);
			b++;
		}
		a++;
	}
	return (1);
}

int	is_valid_echo_paran(char *s)
{
	int	a;

	a = 0;
	if (s && s[a] && s[a] == '-')
	{
		while (s[++a])
		{
			if (s[a] != 'n')
				return (0);
		}
	}
	else
		return (0);
	return (1);
}
