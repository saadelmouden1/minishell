/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_checker.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-azz <isel-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 00:22:56 by isel-azz          #+#    #+#             */
/*   Updated: 2024/10/16 00:22:57 by isel-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_syntax_error(char *input)
{
	if (unclosed_quote(input))
		return (ft_putstr_fd("Syntax Error : unclosed quote\n", 2), 1);
	if (invalid_redirection(input))
		return (ft_putstr_fd("Syntax Error : invalid redirection\n", 2), 1);
	if (invalid_pipe(input))
		return (ft_putstr_fd("Syntax Error : invalid pipe or operator\n", 2), 1);
	if (logical_operators(input))
		return (ft_putstr_fd("Syntax Error :|| and && are not supported\n", 2),
			1);
	return (0);
}

int	unclosed_quote(char *input)
{
	char	quote;

	while (*input)
	{
		if (*input == '\'' || *input == '\"')
		{
			if (quote == *input)
				quote = 0;
			else if (!quote)
				quote = *input;
		}
		input++;
	}
	return (quote != 0);
}

int	invalid_redirection(char *input)
{
	int	sq_count;
	int	dq_count;

	sq_count = 0;
	dq_count = 0;
	while (*input)
	{
		quote_count(*input, &sq_count, &dq_count);
		if ((!(sq_count % 2) && !(dq_count % 2))
			&& (*input == '>' || *input == '<'))
		{
			if (invalid_operator(&input))
				return (1);
		}
		else
			input++;
	}
	return (0);
}

int	invalid_pipe(char *input)
{
	int	sq_count;
	int	dq_count;

	sq_count = 0;
	dq_count = 0;
	if (*input == '|' || *input == '&')
		return (1);
	while (*input)
	{
		quote_count(*input, &sq_count, &dq_count);
		if ((!(sq_count % 2) && !(dq_count % 2)) && *input == '|')
		{
			input++;
			input = skip_spaces(input);
			if (*input == '|' || *input == '<' 
				|| *input == '>' || *input == '\0')
				return (1);
		}
		input++;
	}
	return (0);
}

int	logical_operators(char *input)
{
	int	sq_count;
	int	dq_count;

	sq_count = 0;
	dq_count = 0;
	while (*input)
	{
		quote_count(*input, &sq_count, &dq_count);
		if ((!(sq_count % 2) && !(dq_count % 2))
			&& ((*input == '&' && *(input + 1) == '&')
				|| (*input == '|' && *(input + 1) == '|')))
			return (1);
		input++;
	}
	return (0);
}
