/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string_utils1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-azz <isel-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 00:22:50 by isel-azz          #+#    #+#             */
/*   Updated: 2024/10/16 00:22:51 by isel-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(char *s_1, char *s_2, char *s_3)
{
	int					a;

	a = 0;
	while (s_1[a] && s_2[a] && s_1[a] == s_2[a])
		a += 1;
	if (a == str_size(s_1, '\0')
		&& a == str_size(s_2, '\0'))
		return (1);
	if (s_3)
		return (ft_strcmp(s_1, s_3, NULL));
	return (0);
}

int	str_to_int(char *str)
{
	int					number;
	int					i;

	number = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] <= '9' && str[i] >= '0')
			number = (number * 10) + (str[i] - 48);
		i++;
	}
	return (number);
}

void	free_str_arr(char **arr)
{
	int	i;

	i = 0;
	while (arr[i] != 0)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}
