/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-azz <isel-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 00:23:04 by isel-azz          #+#    #+#             */
/*   Updated: 2024/10/16 00:23:05 by isel-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_space(char *line)
{
	int				a;

	a = 0;
	while (line[a] == ' ' || line[a] == '\t'
		|| line[a] == '\n')
		a += 1;
	if (line[a] == '\0')
		return (1);
	return (0);
}

int	check_line(char **line)
{
	if (*line[0] == '\0' || ft_strcmp(*line, "\n", NULL) || is_space(*line))
	{
		free(*line);
		return (1);
	}
	return (0);
}

char	*str_no_char(char *str, char del)
{
	int				a;
	int				b;
	char			*new_str;

	a = 0;
	b = 0;
	new_str = malloc(str_size(str, '\0') + 1);
	if (!new_str)
		return (NULL);
	while (str[a])
	{
		if (str[a] != del)
			new_str[b++] = str[a];
		a++;
	}
	new_str[b] = '\0';
	return (new_str);
}

int	count_digits(int number)
{
	int	i;

	i = 0;
	if (!number)
		return (1);
	while (number)
	{
		number = number / 10;
		i++;
	}
	return (i);
}

void	free_env_var(char ***arr)
{
	int	i;

	i = 0;
	while (arr[i])
	{
		free(arr[i][0]);
		free(arr[i][1]);
		free(arr[i]);
		i++;
	}
	free(arr);
}
