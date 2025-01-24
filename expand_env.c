/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-azz <isel-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 00:22:14 by isel-azz          #+#    #+#             */
/*   Updated: 2024/10/16 00:22:15 by isel-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*replace_variable_with_value(
		char *old_var, char *__new, int st, int end)
{
	int							size;
	char						*new__;
	int							unsize;

	unsize = str_size(__new, '\0');
	size = st + (str_size(old_var, '\0') - end) + unsize;
	new__ = malloc(size + 1);
	if (!new__)
		return (NULL);
	string_copy(new__, old_var, 0, st);
	string_copy(new__ + st, __new, 0, unsize);
	string_copy(new__ + st + unsize, old_var, end, str_size(old_var, '\0'));
	free(old_var);
	return (new__);
}

char	*expand_variable_in_string(char *var, t_env *env, int a, int *f_hole)
{
	int							hole_size;
	int							c;
	int							b;
	char						*new_var;

	b = a + 1;
	while (var[a] && is_valid_variable_start(var, b, 0))
		b++;
	hole_size = b - a;
	new_var = malloc(hole_size + 1);
	string_copy(new_var, var, a + 1, b);
	if (ft_strcmp(new_var, "?", NULL) && g_var_tn)
		update_env_status(env, g_var_tn, "?=");
	g_var_tn = 0;
	c = find_env_var_index(env, new_var);
	free(new_var);
	*f_hole = 0;
	if (c >= 0)
	{
		new_var = replace_variable_with_value(var, env->parsed_env[c][1], a, b);
		*f_hole = str_size(env->parsed_env[c][1], '\0') + a;
	}
	else
		new_var = replace_variable_with_value(var, "", a, b);
	return (new_var);
}

char	*recursively_expand_variables(
		char *var, t_env *env, int __con, int *f_arr)
{
	char						*new_var;

	while (var[f_arr[0]])
	{
		if (var[f_arr[0]] == 39)
		{
			f_arr[0]++;
			f_arr[1]++;
			while (!(f_arr[2] % 2) && var[f_arr[0]] && var[f_arr[0]] != 39)
				f_arr[0]++;
		}
		if (var[f_arr[0]] == 34)
			f_arr[2]++;
		if (is_valid_variable_start(var, f_arr[0], 1)
			&& ((!(f_arr[2] % 2) && __con) || (f_arr[2] % 2 && !__con)))
			return (new_var = expand_variable_in_string(
					var, env, f_arr[0], &f_arr[0]),
				recursively_expand_variables(new_var,
					env, __con, f_arr));
		f_arr[0]++;
	}
	return (var);
}

char	**refactore_args_array(char **args, int *quick_norm_fix)
{
	int						a;
	int						b;
	int						c;
	char					**new_args;

	a = 0;
	b = 0;
	*quick_norm_fix = 0;
	c = array_str_count(args);
	new_args = malloc((detected_flaws(args) + c + 1) * sizeof(char **));
	if (!new_args)
		return (NULL);
	while (args[a])
	{
		c = is_flawed_str(args[a], 0, 0, 0);
		if (c)
			b = simplified_refactor_thing(new_args, b, args[a], 0);
		else
			new_args[b++] = strcopy(args[a]);
		a++;
	}
	new_args[b] = 0;
	free_str_arr(args);
	return (new_args);
}

void	expand_variables_in_ast(t_ast_node *head, t_env *env)
{
	int							a;
	int							f_arr[3];

	if (head->file_type != FILE_READY && head->args
		&& head->file_type != READ_FROM_APPEND)
	{
		a = -1;
		while (head->args[++a])
			((ft_memset(f_arr, 0, 3 * sizeof(int))),
				(head->args[a] = recursively_expand_variables(
						head->args[a], env, 1, f_arr)));
		head->args = refactore_args_array(head->args, &a);
		while (head->args[a])
		{
			ft_memset(f_arr, 0, 3 * sizeof(int));
			head->args[a] = recursively_expand_variables(
					head->args[a], env, 0, f_arr);
			head->args[a] = rm_quotes(head->args[a], 0, 0, 0);
			a++;
		}
	}
	if (head->left)
		expand_variables_in_ast(head->left, env);
	if (head->right)
		expand_variables_in_ast(head->right, env);
}
