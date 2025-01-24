/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-azz <isel-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 04:03:19 by isel-azz          #+#    #+#             */
/*   Updated: 2023/11/19 19:20:00 by isel-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	char	*s;
	size_t	i;

	s = (char *)src;
	i = 0;
	if (dstsize < 1)
		return (ft_strlen(src));
	while (s[i] && dstsize - 1)
	{
		dst[i] = s[i];
		i++;
		dstsize--;
	}
	dst[i] = '\0';
	return (ft_strlen(src));
}
