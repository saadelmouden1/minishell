/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-azz <isel-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 00:22:48 by isel-azz          #+#    #+#             */
/*   Updated: 2024/10/16 00:22:49 by isel-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	str_size(char *str, char end)
{
	int			a;

	a = 0;
	while (str && str[a] != '\0' && str[a] != end)
		a += 1;
	return (a);
}

int	array_str_count(char **arr)
{
	int			a;

	a = 0;
	while (arr[a] != 0)
		a++;
	return (a);
}

char	*rm_quotes(char *str, int s_q_c, int d_q_c, int a)
{
	char	*new_str;
	int		b;

	b = 0;
	new_str = malloc(str_size(str, '\0') + 1);
	while (str[a])
	{
		if (str[a] == 34 && !(s_q_c % 2))
			d_q_c++;
		else if (str[a] == 39 && !(d_q_c % 2))
			s_q_c++;
		if ((str[a] != 34 || s_q_c % 2)
			&& ((str[a] != 39) || d_q_c % 2))
			new_str[b++] = str[a];
		a++;
	}
	new_str[b] = '\0';
	return (free(str), new_str);
}

char	*strcopy(char *src)
{
	char	*dest;
	int		a;
	int		b;

	a = 0;
	while (src[a])
		a++;
	dest = malloc((a + 1) * sizeof(char));
	if (!dest)
		return (NULL);
	b = 0;
	while (b < a)
	{
		dest[b] = src[b];
		b++;
	}
	dest[b] = '\0';
	return (dest);
}

void	string_copy(char *s1, char *s2, int st, int en)
{
	int	a;

	a = 0;
	while (st < en)
		s1[a++] = s2[st++];
	s1[a] = '\0';
}
