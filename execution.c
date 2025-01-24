/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-azz <isel-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 19:46:57 by sel-moud          #+#    #+#             */
/*   Updated: 2024/10/16 01:24:20 by isel-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	manage_piped_execution(t_ast_node *head, int *piped, t_env *env, int *fd)
{
	int	status;

	if (head->file_type == EXECUTE_FILE)
	{
		piped[8] = 0;
		status = preparation_to_exexcute_command(head->args, fd, piped, env);
	}
	if (head->type == TOKEN_REDIR_IN || head->type == TOKEN_REDIR_OUT
		|| head->type == TOKEN_REDIR_APPEND
		|| head->type == TOKEN_REDIR_HEREDOC)
	{
		return (handle_redirections_cmd(head, piped, env, fd));
	}
	if (head->left)
		status = manage_piped_execution(head->left, piped, env, fd);
	if (head->right)
		status = manage_piped_execution(head->right, piped, env, fd);
	return (status);
}

int	handle_redirections_cmd(t_ast_node *head, int *piped, t_env *env, int *fd)
{
	int	status;

	piped[11] = 1;
	if (head->right)
	{
		status = file_to_redirection_open(head->right, piped, env, 0);
		if ((status || !head->left) && piped[0] > 1)
			piped[0] -= 1;
	}
	if (head->left && head->left->file_type == EXECUTE_FILE && piped[11]
		&& !status)
	{
		piped[8] = 1;
		status = preparation_to_exexcute_command(head->left->args, fd, piped,
				env);
	}
	if (head->left && head->left->file_type == TOKEN_PIPE && piped[11])
		status = manage_piped_execution(head->left, piped, env, fd);
	if (head->left && (head->left->type == TOKEN_REDIR_IN
			|| head->left->type == TOKEN_REDIR_OUT
			|| head->left->type == TOKEN_REDIR_APPEND
			|| head->left->type == TOKEN_REDIR_HEREDOC))
		status = handle_redirections_cmd(head->left, piped, env, fd);
	return (status);
}

int	ast_node_execution(t_ast_node *head, int *piped, t_env *env)
{
	int	fd[2];
	int	status;

	fd[0] = -1;
	fd[1] = -1;
	if (head->file_type == FILE_READY)
	{
		if (head->type == TOKEN_PIPE)
			status = manage_piped_execution(head, piped, env, fd);
		if (head->type == TOKEN_REDIR_IN || head->type == TOKEN_REDIR_OUT
			|| head->type == TOKEN_REDIR_APPEND
			|| head->type == TOKEN_REDIR_HEREDOC)
			status = handle_redirections_cmd(head, piped, env, fd);
	}
	if (head->file_type == EXECUTE_FILE)
		status = preparation_to_exexcute_command(head->args, fd, piped, env);
	status = wait_for_children(status, piped);
	if (piped[6])
		close(piped[1]);
	if (piped[7])
		close(piped[2]);
	if (fd[0] != -1 || fd[1] != -1)
		(close(fd[0]), close(fd[1]));
	return (g_var_tn = 0, status);
}

void	manage_execution_commands(t_ast_node *head, t_env *env, int *status)
{
	int	piped[13];
	int	st;

	init_or_reset_pipe_state(piped, 1);
	count_red_and_pipes(head, piped);
	init_or_reset_pipe_state(piped, 0);
	prepare_ast_nodes_to_execute(head);
	expand_variables_in_ast(head, env);
	st = verify_cmdfile_peresion(head, env->original_env);
	if (!st)
		*status = ast_node_execution(head, piped, env);
}
