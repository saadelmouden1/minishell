/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-azz <isel-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 00:22:58 by isel-azz          #+#    #+#             */
/*   Updated: 2024/10/16 00:22:59 by isel-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*generate_token(t_typetoken type, char *token_value)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->token_type = type;
	new_token->value = ft_strdup(token_value);
	if (!new_token)
		return (free(new_token), NULL);
	new_token->next = NULL;
	return (new_token);
}

void	add_token(t_token **tokens, t_token *new_token)
{
	t_token	*last_token;

	if (!*tokens)
		*tokens = new_token;
	else
	{
		last_token = *tokens;
		while (last_token->next)
			last_token = last_token->next;
		last_token->next = new_token;
	}
}

void	add_word_token(char **start, char **input, t_token **tokens)
{
	char	*word;

	if (*input > *start)
	{
		word = ft_strndup(*start, *input - *start);
		if (word)
		{
			add_token(tokens, generate_token(TOKEN_WORD, word));
			free(word);
		}
		else
			ft_putstr_fd("Error: Malloc failed in handle_word.\n", 2);
	}
}

void	free_tokens(t_token *tokens)
{
	t_token	*placeholder;

	placeholder = tokens;
	while (tokens)
	{
		placeholder = tokens;
		tokens = tokens->next;
		free(placeholder->value);
		free(placeholder);
	}
}

void	quote_status(char c, int *in_quote, char *quote_char)
{
	if (!*in_quote && (c == '\'' || c == '\"'))
	{
		*in_quote = 1;
		*quote_char = c;
	}
	else if (*in_quote && c == *quote_char)
		*in_quote = 0;
}
