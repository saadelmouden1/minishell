/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-azz <isel-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 00:23:01 by isel-azz          #+#    #+#             */
/*   Updated: 2024/10/16 00:23:02 by isel-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	found_special_char(char **input, t_token **tokens)
{
	if (**input == '>')
	{
		if (*(*input + 1) == '>')
		{
			add_token(tokens, generate_token(TOKEN_REDIR_APPEND, ">>"));
			(*input)++;
		}
		else
			add_token(tokens, generate_token(TOKEN_REDIR_OUT, ">"));
	}
	else if (**input == '<')
	{
		if (*(*input + 1) == '<')
		{
			add_token(tokens, generate_token(TOKEN_REDIR_HEREDOC, "<<"));
			(*input)++;
		}
		else
			add_token(tokens, generate_token(TOKEN_REDIR_IN, "<"));
	}
	else if (**input == '|')
		add_token(tokens, generate_token(TOKEN_PIPE, "|"));
	(*input)++;
}

void	found_word(char **input, t_token **tokens)
{
	char	*start;
	char	quote;
	int		quoted;

	quote = '\0';
	quoted = 0;
	start = *input;
	while (**input)
	{
		quote_status(**input, &quoted, &quote);
		if (!quoted && ft_strchr(" \t\n><|", **input))
			break ;
		(*input)++;
	}
	add_word_token(&start, input, tokens);
}

t_token	*tokenize_input(char *input)
{
	t_token	*tokens;

	tokens = NULL;
	while (*input)
	{
		while (*input && ft_strchr(" \t\n", *input))
			input++;
		if (ft_strchr("><|", *input))
			found_special_char(&input, &tokens);
		else
			found_word(&input, &tokens);
	}
	return (tokens);
}
