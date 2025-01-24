/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmand_exec_init.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isel-azz <isel-azz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 19:46:43 by sel-moud          #+#    #+#             */
/*   Updated: 2024/10/16 01:24:02 by isel-azz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sus_dir_chek(char *path, char *f, int *status)
{
	struct stat	s;

	if (f && ft_strcmp(f, ".", NULL))
		*status = 2;
	else if (ft_strcmp(f, "..", NULL) || ft_strcmp(f, ",", ""))
	{
		*status = 1;
		errno = 2;
	}
	else if (!stat(path, &s) && s.st_mode & S_IFDIR)
	{
		*status = 2;
		ft_putstr_fd("   err: this \'", 2);
		ft_putstr_fd(path, 2);
		ft_putendl_fd("\' Is a directory", 2);
		errno = 13;
	}
}

int	specify_what_error_stuff(char *file, int status)
{
	if (status == 1)
	{
		status = get_shell_exit_status(errno);
		ft_putstr_fd("   err:\'", 2);
		ft_putstr_fd(file, 2);
		ft_putstr_fd("\'", 2);
		ft_putendl_fd(strerror(errno), 2);
		return (status);
	}
	else if (status)
	{
		ft_putstr_fd("   minishell(\'", 2);
		ft_putstr_fd(file, 2);
		ft_putendl_fd("\'): go play somwhere", 2);
	}
	return (status);
}

int	verify_cmdfile_peresion(t_ast_node *head, char **env)
{
	int		status;
	char	*path;

	status = 0;
	if (head->args && !check_builtin_cmd(head->args[0])
		&& head->file_type == READ_FILE)
	{
		path = fetch_file_path(head->args[0], env, "PWD", R_OK);
		if (!path)
			status = 0;
		else
		{
			sus_dir_chek(path, head->args[0], &status);
			free(path);
		}
		status = specify_what_error_stuff(head->args[0], status);
	}
	if (!status && head->left)
		status = verify_cmdfile_peresion(head->left, env);
	if (!status && head->right)
		status = verify_cmdfile_peresion(head->right, env);
	return (status);
}

void	prepare_ast_nodes_to_execute(t_ast_node *head)
{
	if (head->type != TOKEN_WORD)
	{
		head->file_type = FILE_READY;
		if (head->type == TOKEN_REDIR_OUT && head->right)
			head->right->file_type = WRITE_FILE;
		if (head->type == TOKEN_REDIR_APPEND && head->right)
			head->right->file_type = WRITE_FILE_APPEND;
		if (head->type == TOKEN_REDIR_IN && head->right)
			head->right->file_type = READ_FILE;
		if (head->type == TOKEN_REDIR_HEREDOC && head->right)
			head->right->file_type = READ_FROM_APPEND;
		if (head->type == TOKEN_PIPE)
		{
			if (head->right)
				head->right->file_type = EXECUTE_FILE;
			if (head->left)
				head->left->file_type = EXECUTE_FILE;
		}
	}
	if (!head->file_type)
		head->file_type = EXECUTE_FILE;
	if (head->left)
		prepare_ast_nodes_to_execute(head->left);
	if (head->right)
		prepare_ast_nodes_to_execute(head->right);
}

void	count_red_and_pipes(t_ast_node *head, int *piped)
{
	head->file_type = 0;
	if (head->type == TOKEN_REDIR_OUT || head->type == TOKEN_REDIR_APPEND)
		piped[4] += 1;
	if (head->type == TOKEN_REDIR_IN || head->type == TOKEN_REDIR_HEREDOC)
		piped[3] += 1;
	if (head->type == TOKEN_PIPE)
		piped[5] += 1;
	if (head->left)
		count_red_and_pipes(head->left, piped);
	if (head->right)
		count_red_and_pipes(head->right, piped);
}
