/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   herdoc_handl.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sel-moud <sel-moud@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/15 19:47:06 by sel-moud          #+#    #+#             */
/*   Updated: 2024/10/15 23:57:37 by sel-moud         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	str_comper(char *s1, char *s2, int max)
{
	int	a;

	a = 0;
	while (a < max && *s1 && *s2 && s1[a] == s2[a])
		a++;
	if (str_size(s1, '\0') == a && a == max)
		return (1);
	return (0);
}

void	exite_herdoc(int a)
{
	(void)a;
	write(1, "\n", 1);
	exit(0);
}

int	is_there_quotes(char *s)
{
	int	a;

	a = 0;
	while (a && s[a])
	{
		if (s[a] == 34 || s[a] == 39)
			return (0);
		a++;
	}
	return (1);
}

void	read_and_write(int std_out, char *limiter, t_env *env,
		int is_expandable)
{
	char	*buf;
	int		f_arr[3];

	limiter = rm_quotes(limiter, 0, 0, 0);
	while (1)
	{
		buf = readline(">> ");
		if (!buf || str_comper(limiter, buf, str_size(buf, '\n')))
		{
			free(buf);
			break ;
		}
		if (is_expandable)
		{
			buf[str_size(buf, '\n')] = '\0';
			ft_memset(f_arr, 0, 3 * sizeof(int));
			buf = recursively_expand_variables(buf, env, 0, f_arr);
			ft_memset(f_arr, 0, 3 * sizeof(int));
			buf = recursively_expand_variables(buf, env, 1, f_arr);
		}
		write(std_out, buf, str_size(buf, '\0'));
		write(std_out, "\n", 1);
		free(buf);
	}
	free(limiter);
}

int	here_doc_execution(char *delmtr, int *piped, t_env *env)
{
	int		out_fd[2];
	pid_t	pid;
	int		status;

	pipe(out_fd);
	pid = fork();
	signal(SIGINT, SIG_IGN);
	if (!pid)
	{
		signal(SIGINT, exite_herdoc);
		close(out_fd[0]);
		read_and_write(out_fd[1], delmtr, env, is_there_quotes(delmtr));
		exit(1);
	}
	waitpid(pid, &status, 0);
	close(out_fd[1]);
	piped[1] = out_fd[0];
	piped[9] = (WEXITSTATUS(status)) - 1;
	if (piped[9] < 0)
		piped[9] += 2;
	piped[11] = status;
	return (piped[9]);
}
