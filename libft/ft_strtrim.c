/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-azz <isel-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 22:15:45 by isel-azz          #+#    #+#             */
/*   Updated: 2023/11/25 15:30:03 by isel-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	charcheck(char const *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (0);
}

static void	fill(char *res, char const *s1, int slen, int i)
{
	int	j;

	j = 0;
	while (i < slen)
		res[j++] = s1[i++];
	res[j] = 0;
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*res;
	int		i;
	int		slen;
	int		j;

	i = 0;
	j = 0;
	if (!s1 || (!s1 && !set))
		return (NULL);
	if (!set)
		return (ft_strdup(s1));
	slen = ft_strlen(s1);
	if (!*s1)
		return (ft_strdup(""));
	if (!*set)
		return (ft_strdup(s1));
	while (s1[i] && charcheck(set, s1[i]))
		i++;
	while (slen > i && charcheck(set, s1[slen - 1]))
		slen--;
	res = (char *)malloc((slen - i + 1) * sizeof(char));
	if (!res)
		return (NULL);
	fill(res, s1, slen, i);
	return (res);
}
