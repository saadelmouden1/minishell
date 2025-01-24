/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-azz <isel-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 00:22:33 by isel-azz          #+#    #+#             */
/*   Updated: 2024/10/16 00:22:34 by isel-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast_node	*parse_command(t_token **tokens)
{
	t_ast_node	*command_node;
	int			args_count;

	command_node = new_ast_node(TOKEN_WORD);
	args_count = count_args((*tokens));
	command_node->args = malloc((args_count + 1) * sizeof(char *));
	if (!command_node->args)
		return (NULL);
	fill_args(command_node, tokens, args_count);
	return (command_node);
}

t_ast_node	*create_file_node(t_token *token)
{
	t_ast_node	*file_node;

	file_node = malloc(sizeof(t_ast_node));
	if (!file_node)
		return (NULL);
	file_node->type = token->token_type;
	file_node->args = malloc(2 * sizeof(char *));
	if (!file_node->args)
		return (free(file_node), NULL);
	file_node->args[0] = token->value;
	file_node->args[1] = NULL;
	file_node->left = NULL;
	file_node->right = NULL;
	free(token);
	return (file_node);
}

t_ast_node	*parse_redirection(t_token **tokens)
{
	t_ast_node	*red_node;
	t_token		*next_token;
	t_token		*placeholder;

	if (!*tokens)
		return (NULL);
	placeholder = *tokens;
	if ((*tokens)->token_type >= TOKEN_REDIR_IN
		&& (*tokens)->token_type <= TOKEN_REDIR_HEREDOC)
		return (create_redir_node(tokens, placeholder));
	while (*tokens && (*tokens)->next)
	{
		next_token = (*tokens)->next;
		if ((*tokens)->next->token_type >= TOKEN_REDIR_IN
			&& (*tokens)->next->token_type <= TOKEN_REDIR_HEREDOC)
		{
			red_node = new_ast_node((*tokens)->next->token_type);
			(*tokens)->next = next_token->next->next;
			red_node->left = parse_redirection(&placeholder);
			red_node->right = create_file_node(next_token->next);
			return (free(next_token->value), free(next_token), red_node);
		}
		*tokens = next_token;
	}
	return (parse_command(&placeholder));
}

t_ast_node	*parse_pipe(t_token **tokens)
{
	t_token		*placeholder;
	t_token		*next_token;
	t_ast_node	*pipe_node;

	placeholder = *tokens;
	while (*tokens && (*tokens)->next)
	{
		next_token = (*tokens)->next;
		if ((*tokens)->next->token_type == TOKEN_PIPE)
		{
			pipe_node = new_ast_node((*tokens)->next->token_type);
			(*tokens)->next = NULL;
			pipe_node->left = parse_redirection(&placeholder);
			pipe_node->right = parse_pipe(&(next_token->next));
			free(next_token->value);
			free(next_token);
			return (pipe_node);
		}
		*tokens = next_token;
	}
	return (parse_redirection(&placeholder));
}

t_ast_node	*parse(t_token **tokens)
{
	if (!tokens || !*tokens)
		return (NULL);
	return (parse_pipe(tokens));
}
