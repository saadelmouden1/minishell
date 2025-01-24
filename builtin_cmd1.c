/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cmd1.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-moud <sel-moud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 19:46:16 by sel-moud          #+#    #+#             */
/*   Updated: 2024/10/15 19:48:38 by sel-moud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_new_pwd_in_env(char *new, t_env *env, int c)
{
	char	**exp_pwd;
	int		a;
	int		b;

	a = 0;
	b = 4;
	c = 0;
	exp_pwd = malloc(3 * sizeof(char *));
	if (!exp_pwd)
		return ;
	exp_pwd[0] = strcopy("export");
	exp_pwd[1] = malloc(str_size(new, '\0') + 5);
	if (!exp_pwd[1])
		return ;
	string_copy(exp_pwd[1], "PWD=", 0, 4);
	while (new[a])
		exp_pwd[1][b++] = new[a++];
	exp_pwd[1][b] = '\0';
	exp_pwd[2] = 0;
	unset_or_export_cmd(exp_pwd, env, NULL, &a);
	free_str_arr(exp_pwd);
}

int	change_current_directory(char *path, t_env *env)
{
	int	status;
	int	a;

	if (!path || !sizeof(path, '\0'))
	{
		a = find_env_var_index(env, "HOME");
		if (a >= 0)
			status = chdir(env->parsed_env[a][1]);
		else
			status = -1;
	}
	else
		status = chdir(path);
	return (status);
}

char	*get_current_working_directory(int size, int tries, int fd)
{
	char	*buff;

	buff = malloc(size);
	if (!buff)
		return (NULL);
	if (!getcwd(buff, size))
	{
		free(buff);
		if (tries < 10)
			return (get_current_working_directory(size + 50, tries + 1, fd));
		else
		{
			ft_putendl_fd("  err:cd() / pwd(): getcwd():you are lost", fd);
			return (NULL);
		}
	}
	return (buff);
}

char	***sort_tha_array(char ***_array, int _si)
{
	int		a;
	int		b;
	int		c;
	char	**temp_p;

	a = 0;
	while (a < _si - 1)
	{
		b = a + 1;
		c = string_weight_compare(_array[a][0], _array[b][0]);
		if (c)
		{
			temp_p = _array[a];
			_array[a] = _array[b];
			_array[b] = temp_p;
		}
		a++;
	}
	if (check_array_arrangment(_array, _si))
		return (_array);
	return (sort_tha_array(_array, _si));
}

void	print_export_declaration_to_fd(t_env *env, int *out_fd)
{
	char	***new_arr;
	int		a;

	a = 0;
	while (env->parsed_env[a] != 0)
		a++;
	if (!a)
		return ;
	new_arr = duplicate_env_structure(env, a, 'F', -1);
	new_arr[a] = 0;
	new_arr = sort_tha_array(new_arr, a);
	print_export_vars(new_arr, a, out_fd[1]);
	free_env_var(new_arr);
}
