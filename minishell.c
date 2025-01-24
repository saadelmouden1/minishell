/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-azz <isel-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 00:22:30 by isel-azz          #+#    #+#             */
/*   Updated: 2024/10/16 01:21:52 by isel-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*check_tokenize(char *input)
{
	char	*trimmed_input;
	t_token	*tokens;

	trimmed_input = ft_strtrim(input, " \t\n\v\f\r");
	free(input);
	if (!trimmed_input)
		return (NULL);
	if (check_syntax_error(trimmed_input))
		return (free(trimmed_input), NULL);
	tokens = tokenize_input(trimmed_input);
	free(trimmed_input);
	return (tokens);
}

void	main_loop(t_env *env)
{
	char		*input;
	t_token		*tokens;
	t_ast_node	*ast;
	int			stat;

	while (1)
	{
		stat = 0;
		input = readline("minishell> ");
		if (!input)
			break ;
		if (check_line(&input))
			continue ;
		add_history(input);
		tokens = check_tokenize(input);
		if (!tokens)
			stat = 258;
		if (!stat)
		{
			ast = parse(&tokens);
			manage_execution_commands(ast, env, &stat);
			free_ast(ast);
		}
		update_env_status(env, stat, "?=");
	}
}

int	main(int argc, char **argv, char **original_env)
{
	t_env	*env;

	setup_signal_handlers();
	env = malloc(sizeof(t_env));
	if (argc == 1 && argv && original_env 
		&& shell_initializing_with_env(env, original_env))
	{
		main_loop(env);
		cleanup_exit(env, 0);
	}
}
