/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-azz <isel-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 00:22:22 by isel-azz          #+#    #+#             */
/*   Updated: 2024/10/16 00:22:23 by isel-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_variable_start(char *str, int index, int con)
{
	if ((con && str[index] == '$'
			&& str[index + 1]
			&& str[index + 1] != '$'
			&& !ft_isspace(str[index + 1])
			&& (ft_isalnum(str[index + 1])
				|| str[index + 1] == '_'
				|| str[index + 1] == '?'))
		|| (!con && str[index]
			&& str[index] != '$'
			&& !ft_isspace(str[index])
			&& (ft_isalnum(str[index])
				|| str[index] == '_'
				|| str[index] == '?')))
		return (1);
	return (0);
}

void	append_env_var(char *var, t_env *env)
{
	int				b;
	int				c;
	int				d;
	int				o;
	char			*env_var;

	c = str_size(var, '+');
	d = str_size(var, '\0') - c - 1;
	env_var = malloc(c + 1);
	string_copy(env_var, var, 0, c);
	o = find_env_var_index(env, env_var);
	free(env_var);
	if (o >= 0)
	{
		b = str_size(env->original_env[o], '\0');
		env_var = malloc(b + d + 1);
		if (!env_var)
			return ;
		string_copy(env_var, env->original_env[o], 0, b);
		string_copy(env_var + b, var, c + 2, str_size(var, '\0'));
	}
	else
		env_var = str_without_char(var, '+');
	replace_env_var(env_var, env);
	free(env_var);
}

int	find_env_var_index(t_env *env, char *name)
{
	int				a;

	a = 0;
	while (env->parsed_env[a] != 0)
	{
		if (ft_strcmp(env->parsed_env[a][0], name, NULL))
			return (a);
		a++;
	}
	return (-1);
}
