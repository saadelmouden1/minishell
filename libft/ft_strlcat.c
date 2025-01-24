/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-azz <isel-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 23:19:07 by isel-azz          #+#    #+#             */
/*   Updated: 2023/11/24 01:03:59 by isel-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	len;

	i = 0;
	if (!dst && !dstsize)
		return (ft_strlen(src));
	len = ft_strlen(dst);
	if (len > dstsize)
		return (dstsize + ft_strlen(src));
	if (!dstsize)
		return (ft_strlen(src));
	while (src[i] && i + len < dstsize - 1)
	{
		dst[i + len] = src[i];
		i++;
	}
	dst[i + len] = '\0';
	return (ft_strlen(src) + len);
}
