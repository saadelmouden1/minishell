/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-azz <isel-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 00:22:17 by isel-azz          #+#    #+#             */
/*   Updated: 2024/10/16 00:22:18 by isel-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	simplified_refactor_thing(char **array, int index, char *str, int in)
{
	int							size;

	if (!str[in] || in > str_size(str, '\0'))
		return (index);
	while (str[in] && str[in] == ' ')
		in++;
	size = str_size(str + in, ' ');
	if (size > str_size(str + in, 34))
		size = str_size(str + in, 34);
	if (size > str_size(str + in, 39))
		size = str_size(str + in, 39);
	if (str[in] == 34)
		size = str_size(str + in + 1, 34) + 2;
	if (str[in] == 39)
		size = str_size(str + in + 1, 39) + 2;
	if ((in + size) > str_size(str, '\0'))
		return (index);
	array[index] = malloc(size + 1);
	if (!array[index])
		return (index);
	string_copy(array[index], str, in, in + size);
	return (simplified_refactor_thing(array, index + 1, str, in + size));
}

int	sus_getit_right(char *str, int s_q, int d_q)
{
	int					a;

	a = 0;
	(void)s_q;
	while (str[a])
	{
		if (str[a] == 34 || str[a] == 39
			|| str[a] == '$')
			d_q++;
		a++;
	}
	return (d_q);
}

int	is_flawed_str(char *str, int a, int b, int res)
{
	int					si_q;
	int					do_q;

	si_q = 0;
	do_q = 0;
	while (str[a])
	{
		if (str[a] == 34)
			do_q++;
		else if (str[a] == 39)
			si_q++;
		else if (!(si_q % 2) && !(do_q % 2))
		{
			if (str[a] == ' ')
			{
				if (b)
					res++;
				b = 0;
			}
			else
				b = 1;
		}
		a++;
	}
	return (res);
}

int	detected_flaws(char **array)
{
	int			a;
	int			res;

	res = 0;
	a = 0;
	while (array[a])
	{
		res += is_flawed_str(array[a], 0, 0, 0);
		res += sus_getit_right(array[a], 0, 0);
		a++;
	}
	return (res);
}
