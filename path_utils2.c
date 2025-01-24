/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-moud <sel-moud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 19:47:29 by sel-moud          #+#    #+#             */
/*   Updated: 2024/10/16 00:03:34 by sel-moud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	find_substr_index(char **haystack, char *needle, int n)
{
	int	a;
	int	b;

	a = 0;
	while (haystack[a] != 0)
	{
		if (haystack[a][0] == needle[0])
		{
			b = 0;
			while (haystack[a][b] && haystack[a][b] == needle[b])
			{
				if (b == n - 1)
					return (a);
				b++;
			}
		}
		a++;
	}
	return (-1);
}

int	is_path_accessible(char *path, int mode)
{
	if (access(path, mode) < 0)
		return (0);
	return (1);
}

void	increment_path_index(char *env_v, int *indx, int *a)
{
	if (indx[3])
	{
		indx[1] += 1;
		*a = indx[1];
		while (env_v[indx[1]] != '\0' && env_v[indx[1]] != ':')
			indx[1] += 1;
	}
	else
		*a = indx[1];
}

int	count_substrings(char *s, char del)
{
	int	a;
	int	b;
	int	res;

	a = 0;
	b = 1;
	res = 0;
	while (s && s[a])
	{
		if (s[a] != del)
		{
			if (b)
				res += 1;
			b = 0;
		}
		else
			b = 1;
		a += 1;
	}
	return (res);
}
