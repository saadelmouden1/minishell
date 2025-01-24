/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-azz <isel-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 00:22:35 by isel-azz          #+#    #+#             */
/*   Updated: 2024/10/16 00:22:36 by isel-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_ast_node	*new_ast_node(t_typetoken type)
{
	t_ast_node	*new_node;

	new_node = malloc(sizeof(t_ast_node));
	if (!new_node)
		return (NULL);
	new_node->type = type;
	new_node->args = NULL;
	new_node->left = NULL;
	new_node->right = NULL;
	return (new_node);
}

t_ast_node	*create_redir_node(t_token **tokens, t_token *placeholder)
{
	t_ast_node	*red_node;

	red_node = new_ast_node((*tokens)->token_type);
	*tokens = (*tokens)->next->next;
	red_node->left = parse_redirection(tokens);
	red_node->right = create_file_node(placeholder->next);
	free(placeholder->value);
	free(placeholder);
	return (red_node);
}

int	count_args(t_token *current)
{
	int	count;

	count = 0;
	while (current && current->token_type == TOKEN_WORD)
	{
		count++;
		current = current->next;
	}
	return (count);
}

void	fill_args(t_ast_node *node, t_token **tokens, int args_count)
{
	t_token	*placeholder;
	int		i;

	i = 0;
	while (i < args_count)
	{
		node->args[i] = ft_strdup((*tokens)->value);
		placeholder = *tokens;
		*tokens = (*tokens)->next;
		free(placeholder->value);
		free(placeholder);
		i++;
	}
	node->args[args_count] = NULL;
}

void	free_ast(t_ast_node *node)
{
	int				i;

	i = 0;
	if (!node)
		return ;
	if (node->type == TOKEN_WORD && node->args)
	{
		while (node->args && node->args[i])
		{
			free(node->args[i]);
			i++;
		}
		free(node->args);
	}
	free_ast(node->left);
	free_ast(node->right);
	free(node);
}
