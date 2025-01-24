/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_check_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-azz <isel-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 00:22:53 by isel-azz          #+#    #+#             */
/*   Updated: 2024/10/16 00:22:54 by isel-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*skip_spaces(char *input)
{
	while (*input && (*input == ' ' || *input == '\t'))
		input++;
	return (input);
}

void	quote_count(char c, int *sq_count, int *dq_count)
{
	if (c == 34)
		(*dq_count)++;
	else if (c == 39)
		(*sq_count)++;
}

int	invalid_operator(char **input)
{
	char	*start;

	start = *input;
	(*input)++;
	if (*start == **input)
		(*input)++;
	*input = skip_spaces(*input);
	if (**input == '\0' || **input == '>'
		|| **input == '<' || **input == '|' || **input == '/')
		return (1);
	return (0);
}
