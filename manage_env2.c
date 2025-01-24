/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_env2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-azz <isel-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 00:22:27 by isel-azz          #+#    #+#             */
/*   Updated: 2024/10/16 00:22:28 by isel-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	replace_env_var(char *var, t_env *env)
{
	int				c;
	int				o;
	char			*env_var;

	c = str_size(var, '=');
	env_var = malloc(c + 1);
	if (!env_var)
		return ;
	string_copy(env_var, var, 0, c);
	o = find_env_var_index(env, env_var);
	if (o >= 0)
		remove_env_entry(env, o);
	free(env_var);
	if (c > 0 && c < str_size(var, '\0') - 1)
		add_env_entry(env, var, c, 1);
	else if (var[c] == '=')
		add_env_entry(env, var, c, 0);
	else if (c == str_size(var, '\0'))
		add_env_entry(env, var, c, -1);
}

void	update_env_status(t_env *env, int status, char *start)
{
	char					*var;
	int						a;

	a = count_digits(status) + str_size(start, '\0') + 1;
	var = malloc(a);
	if (!var)
		return ;
	string_copy(var, start, 0, str_size(start, '\0'));
	var[--a] = '\0';
	if (!status)
		var[--a] = '0';
	while (status)
	{
		var[--a] = (status % 10) + 48;
		status /= 10;
	}
	replace_env_var(var, env);
	free(var);
}

char	*str_without_char(char *str, char del)
{
	int				a;
	int				b;
	char			*new_str;

	a = 0;
	b = 0;
	new_str = malloc(str_size(str, '\0') + 1);
	if (!new_str)
		return (NULL);
	while (str[a])
	{
		if (str[a] != del)
			new_str[b++] = str[a];
		a++;
	}
	new_str[b] = '\0';
	return (new_str);
}
