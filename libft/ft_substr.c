/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-azz <isel-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 03:58:19 by isel-azz          #+#    #+#             */
/*   Updated: 2023/11/14 04:55:06 by isel-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	fill(char const *s, char *res, int start, size_t len)
{
	int	i;

	i = -1;
	while (++i < (int)len && s[i + start])
		res[i] = s[i + start];
	res[i] = '\0';
}

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char		*res;
	size_t		slen;

	if (!s)
		return (NULL);
	slen = ft_strlen(s);
	if (!*s || start > slen)
		return (ft_strdup(""));
	if (len > slen)
		return (ft_substr(s, start, slen - start));
	if ((slen - start) < len)
		res = (char *)malloc(sizeof(char) * (slen - start) + 1);
	else
		res = (char *)malloc(sizeof(char) * len + 1);
	if (!res)
		return (NULL);
	fill(s, res, start, len);
	return (res);
}
