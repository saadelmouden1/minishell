/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-moud <sel-moud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 19:47:47 by sel-moud          #+#    #+#             */
/*   Updated: 2024/10/16 00:06:18 by sel-moud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_shell_exit_status(int err)
{
	if (err == 2)
		return (127);
	else if (err == 13)
		return (126);
	return (err);
}

void	cleanup_exit(t_env *env, int stat)
{
	if (env)
	{
		free_str_arr(env->original_env);
		free_env_var(env->parsed_env);
		free(env);
	}
	exit(stat);
}
