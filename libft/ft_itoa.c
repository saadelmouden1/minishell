/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-azz <isel-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/12 18:40:48 by isel-azz          #+#    #+#             */
/*   Updated: 2023/11/24 00:58:00 by isel-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	chars_num(int nbr)
{
	int	count;

	count = 0;
	while (nbr)
	{
		count++;
		nbr = nbr / 10;
	}
	return (count);
}

static void	fill(char *res, int nbr, int count, int n)
{
	while (count--)
	{
		res[count] = (nbr % 10) + 48;
		nbr = nbr / 10;
	}
	if (n < 0)
		res[0] = '-'; 
}

char	*ft_itoa(int n)
{
	char	*res;
	int		count;
	int		nbr;

	nbr = n;
	count = 0;
	if (n == 0)
		return (ft_strdup("0"));
	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	if (nbr < 0)
	{
		count++;
		nbr *= -1;
	}
	count += chars_num(nbr);
	res = (char *)malloc(count * sizeof(char) + 1);
	if (!res)
		return (NULL);
	res[count] = 0;
	fill(res, nbr, count, n);
	return (res);
}
