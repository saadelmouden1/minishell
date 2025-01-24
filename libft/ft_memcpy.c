/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-azz <isel-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 23:41:33 by isel-azz          #+#    #+#             */
/*   Updated: 2023/11/25 17:12:47 by isel-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*dstp;
	unsigned char	*srcp;

	srcp = (unsigned char *) src;
	dstp = (unsigned char *) dst;
	if (src == dst)
		return (dst);
	if (n && !src && !dst)
		return (dst);
	while (n > 0)
	{
		*dstp = *srcp;
		dstp++;
		srcp++;
		n--;
	}
	return (dst);
}
