/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-azz <isel-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 23:26:41 by isel-azz          #+#    #+#             */
/*   Updated: 2023/11/27 14:27:53 by isel-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	unsigned long long	result;
	int					i;
	int					sign;
	int					ret;

	sign = 1;
	i = 0;
	result = 0;
	while (str[i] && ((str[i] >= 9 && str[i] <= 13) || str[i] == 32))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (ft_isdigit(str[i]))
	{
		result = result * 10 + str[i++] - 48;
		if ((result > 9223372036854775807) && sign == -1)
			return (0);
		else if ((result >= 9223372036854775807) && sign == 1)
			return (-1);
	}
	return (ret = result, ret * sign);
}
