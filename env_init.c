/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-moud <sel-moud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 23:52:02 by isel-azz          #+#    #+#             */
/*   Updated: 2024/10/15 23:55:19 by sel-moud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**duplicate_env_varibles(char **env)
{
	int		a;
	int		b;
	char	**new_env;

	a = 0;
	b = 0;
	while (env[a])
		a++;
	new_env = malloc((a + 1) * sizeof(char **));
	if (!new_env)
		return (NULL);
	while (b < a)
	{
		new_env[b] = strcopy(env[b]);
		b++;
	}
	new_env[b] = NULL;
	return (new_env);
}

int	env_structer_shell_initializing(t_env *env, char **original_env, int a)
{
	int	b;
	int	c;

	env->original_env = duplicate_env_varibles(original_env);
	while (original_env[a])
		a++;
	env->parsed_env = malloc((a + 1) * sizeof(char ***));
	if (!env->parsed_env)
		return (0);
	b = -1;
	while (++b < a)
	{
		c = str_size(original_env[b], '=');
		env->parsed_env[b] = malloc(2 * sizeof(char **));
		env->parsed_env[b][0] = malloc(c * sizeof(char *));
		env->parsed_env[b][1] = malloc((str_size(original_env[b], '\0') - c)
				* sizeof(char *));
		if (!env->parsed_env[b] || !env->parsed_env[b][0]
			|| !env->parsed_env[b][1])
			return (0);
		string_copy(env->parsed_env[b][0], original_env[b], 0, c);
		string_copy(env->parsed_env[b][1], original_env[b], c + 1,
			str_size(original_env[b], '\0'));
	}
	return (env->parsed_env[b] = 0, 1);
}

void	initialize_default_variables(t_env *env, int a)
{
	char	*new_pwd;

	a = find_env_var_index(env, "SHELL");
	if (a >= 0)
		remove_env_entry(env, a);
	replace_env_var("SHELL=minishell", env);
	replace_env_var("?=0", env);
	a = find_env_var_index(env, "PWD");
	new_pwd = get_current_working_directory(100, 5, 2);
	if (new_pwd)
	{
		if (a >= 0)
			remove_env_entry(env, a);
		set_new_pwd_in_env(new_pwd, env, a);
		free(new_pwd);
	}
}

int	shell_initializing_with_env(t_env *env, char **original_env)
{
	int	index;
	int	status;
	int	a;

	if (!env)
		return (0);
	status = env_structer_shell_initializing(env, original_env, 0);
	a = find_env_var_index(env, "SHLVL");
	index = 0;
	if (a >= 0)
		index = str_to_int(env->parsed_env[a][1]);
	update_env_status(env, index + 1, "SHLVL=");
	initialize_default_variables(env, 0);
	return (status);
}
