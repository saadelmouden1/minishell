/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-azz <isel-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 03:20:10 by isel-azz          #+#    #+#             */
/*   Updated: 2023/11/19 19:21:42 by isel-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned char	*s1p;
	unsigned char	*s2p;
	size_t			i;

	s1p = (unsigned char *)s1;
	s2p = (unsigned char *)s2;
	i = 0;
	while (i < n)
	{
		if (s1p[i] != s2p[i])
			return (s1p[i] - s2p[i]);
		i++;
	}
	return (0);
}
