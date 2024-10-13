# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <dirent.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <signal.h>
# include <errno.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>

# define READ_FILE 10
# define READ_FROM_APPEND 15
# define WRITE_FILE 20
# define WRITE_FILE_APPEND 30
# define EXECUTE_FILE 40
# define FILE_READY 50

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HEREDOC,
	TOKEN_ENV_VAR,
}	t_token_type;


typedef struct s_ast_node
{
	t_token_type		type;
	int					file_type;
	char				**args;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}	t_ast_node;

int g_var_tn;

typedef struct s_env
{
    char **original_env;
    char ***parsed_env;
}   t_env;

int handle_redirections_cmd(t_ast_node *head, int *piped, t_env *env, int *fd);
int manage_piped_execution(t_ast_node * head,int *piped ,t_env *env, int *fd);
int check_builtin_cmd(char *cmd);
char	*str_without_char(char *str, char del);
void	replace_env_var(char *var, t_env *env);
int find_env_var_index(t_env *env, char *name);
void	free_string_array(char **arr);
int	detected_flaws(char **array);
int	simplified_refactor_thing(char **array, int index, char *str, int in);
int	is_flawed_str(char *str, int a, int b, int res);
char	*remove_quotes_from_str(char *str, int si_q_c, int do_q_c, int a);


int	ft_isalpha(int c)
{
	if ((c >= 65 && c <= 90) || (c >= 97 && c <= 122))
		return (1);
	return (0);
}

int	ft_isalnum(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
		|| (c >= '0' && c <= '9'))
		return (1);
	return (0);
}

void	*ft_memset(void *s, int c, size_t n)
{
	size_t			i;
	unsigned char	*obj;

	i = 0;
	obj = (unsigned char *)s;
	while (i < n)
	{
		obj[i] = (unsigned char)c;
		i++;
	}
	return (obj);
}

void	free_environment_variables(char ***array)
{
	int				a;

	a = 0;
	while (array[a])
	{
		free(array[a][0]);
		free(array[a][1]);
		free(array[a]);
		a += 1;
	}
	free(array);
}
int	count_strings_in_array(char **arr)
{
	int			a;

	a = 0;
	while (arr[a] != 0)
		a++;
	return (a);
}
int	count_digits_in_int(int num)
{
	int			a;

	a = 0;
	if (!num)
		return (1);
	while (num)
	{
		num /= 10;
		a++;
	}
	return (a);
}
int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}


char	*strcopy(char *src)
{
	int					a;
	int					b;
	char				*dest;

	b = 0;
	while (src[b])
		b += 1;
	dest = malloc(b + 1);
	if (!dest)
		return (NULL);
	a = 0;
	while (a < b)
	{
		dest[a] = src[a];
		a += 1;
	}
	dest[a] = '\0';
	return (dest);
}

// char **duplicate_env_varibles(char **env)
// {
//     int a;
//     int b;
//     char **new_env;
    
//     a = 0;
//     b = 0;
//     while(env[a])
//         a++;
//     new_env = malloc((a + 1) *sizeof(char **));
//     if(!new_env)
//         return NULL;
//     while(b < a)
//     {
//         new_env[b] = strcopy(env[b]);
//         b++;
//     }
//     new_env[b] = NULL;
//     return new_env;
// }

void	s_strcopy(char *s_1, char *s_2, int st, int en)
{
	int				a;

	a = 0;
	while (st < en)
		s_1[a++] = s_2[st++];
	s_1[a] = '\0';
}

int	sizeof_str(char *str, char end)
{
	int			a;

	a = 0;
	while (str && str[a] != '\0' && str[a] != end)
		a += 1;
	return (a);
}

// int env_structer_shell_initializing(t_env *env, char **original_env, int a)
// {

//     int b;
//     int c;

//     env->original_env = duplicate_env_varibles(original_env);
//     while(original_env[a])
//         a++;
//     env->parsed_env = malloc((a +1) * sizeof(char ***));
//     if(!env->parsed_env)
//         return 0;
//     b = -1;
//     while(++b < a)
//     {
//         c = sizeof_str(original_env[b], '=');
//         env->parsed_env[b] = malloc(2 * sizeof(char **));
//         env->parsed_env[b][0] = malloc(c * sizeof(char *));
//         env->parsed_env[b][1] = malloc((sizeof_str(original_env[b], '\0') - c)* sizeof(char *));
//         if(!env->parsed_env[b] || !env->parsed_env[b][0] || !env->parsed_env[b][1])
//         return 0;
//         s_strcopy(env->parsed_env[b][0], original_env[b], 0, c);
//        s_strcopy(env->parsed_env[b][1], original_env[b], c+1, sizeof_str(original_env[b],'\0'));
//     }
//     return (env->parsed_env[b] =0,1);
// }   
int	str_cmp(char *s_1, char *s_2, char *s_3)
{
	int					a;

	a = 0;
	while (s_1[a] && s_2[a] && s_1[a] == s_2[a])
		a += 1;
	if (a == sizeof_str(s_1, '\0')
		&& a == sizeof_str(s_2, '\0'))
		return (1);
	if (s_3)
		return (str_cmp(s_1, s_3, NULL));
	return (0);
}

// int	find_env_var_index(t_env *env, char *name)
// {
// 	int				a;

// 	a = 0;
// 	while (env->parsed_env[a] != 0)
// 	{
// 		if (str_cmp(env->parsed_env[a][0], name, NULL))
// 			return (a);
// 		a++;
// 	}
// 	return (-1);
// }

int	string_to_int(char *str)
{
	int					num;
	int					a;

	num = 0;
	a = 0;
	while (str[a])
	{
		if (str[a] <= '9' && str[a] >= '0')
			num = (num * 10) + (str[a] - 48);
		a++;
	}
	return (num);
}


// int shell_initializing_with_env(t_env *env, char **original_env)
// {
//     int index;
//     int status;
//     int a;

    
//     if(!env)
//     return 0;
//     status = env_structer_shell_initializing(env, original_env, 0);
//     a = find_env_var_index(env, "SHLVL");
//     index = 0;
//     if(a >= 0)
//         index = string_to_int(env->parsed_env[a][1]);
    

//     return(status);

// }
//envirement part code////////
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

	c = sizeof_str(var, '+');
	d = sizeof_str(var, '\0') - c - 1;
	env_var = malloc(c + 1);
	s_strcopy(env_var, var, 0, c);
	o = find_env_var_index(env, env_var);
	free(env_var);
	if (o >= 0)
	{
		b = sizeof_str(env->original_env[o], '\0');
		env_var = malloc(b + d + 1);
		if (!env_var)
			return ;
		s_strcopy(env_var, env->original_env[o], 0, b);
		s_strcopy(env_var + b, var, c + 2, sizeof_str(var, '\0'));
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
		if (str_cmp(env->parsed_env[a][0], name, NULL))
			return (a);
		a++;
	}
	return (-1);
}


///

char	**copy_env_exclude_index(char **env, int a, int a_2, int d)
{
	char				**new_thing;
	int					b;
	int					c;

	b = 0;
	c = 0;
	new_thing = malloc((a + a_2) * sizeof(char **));
	if (!new_thing)
		return (NULL);
	while (b < a)
	{
		if (b != d)
			new_thing[c++] = strcopy(env[b]);
		b++;
	}
	if (d >= 0)
		new_thing[c] = 0;
	free_string_array(env);
	return (new_thing);
}

char	***duplicate_env_structure(t_env *env, int a, int a_2, int d)
{
	int				b;
	int				c;
	char			***new_thing;

	b = 0;
	c = 0;
	new_thing = malloc((a + a_2) * sizeof(char ***));
	while (b < a)
	{
		if (b != d)
		{
			new_thing[c] = malloc(2 * sizeof(char **));
			if (!new_thing[c])
				return (NULL);
			new_thing[c][0] = strcopy(env->parsed_env[b][0]);
			new_thing[c][1] = strcopy(env->parsed_env[b][1]);
			c++;
		}
		b++;
	}
	if (d >= 0)
		new_thing[c] = 0;
	if (a_2 != 'F')
		free_environment_variables(env->parsed_env);
	return (new_thing);
}

void	remove_env_entry(t_env *env, int c)
{
	int				a;

	a = 0;
	while (env->parsed_env[a] != 0)
		a++;
	if (a > 0)
	{
		env->parsed_env = duplicate_env_structure(env, a, 0, c);
		env->original_env = copy_env_exclude_index(env->original_env, a, 0, c);
	}
}

void	update_env_value_at_index(t_env *env, char *cmd, int a, int con)
{
	int				b;
	int				c;

	b = sizeof_str(cmd, '=');
	c = sizeof_str(cmd, '\0') - b;
	if (con > 0)
	{
		env->parsed_env[a][1] = malloc(c + 1);
		if (!env->parsed_env[a][1])
			return ;
		s_strcopy(env->parsed_env[a][1], cmd, b + 1, b + c);
	}
	else
	{
		env->parsed_env[a][1] = malloc(3);
		if (!env->parsed_env[a][1])
			return ;
		if (!con)
			env->parsed_env[a][1][0] = '\0';
		else
			s_strcopy(env->parsed_env[a][1], "F1", 0, 2);
	}
}

void	add_env_entry(t_env *env, char *cmd, int b, int con)
{
	int				a;
	int				c;

	a = 0;
	c = 2;
	if (con)
		c = sizeof_str(cmd, '\0') - b;
	while (env->parsed_env[a] != 0)
		a++;
	env->parsed_env = duplicate_env_structure(env, a, 2, -1);
	env->original_env = copy_env_exclude_index(env->original_env, a, 2, -1);
	env->original_env[a] = malloc(b + c + 1);
	env->parsed_env[a] = malloc(2 * sizeof(char *));
	env->parsed_env[a][0] = malloc(b + 1);
	if (!env->parsed_env[a] || !env->parsed_env[a][0] || !env->original_env[a])
		return ;
	s_strcopy(env->parsed_env[a][0], cmd, 0, b);
	if (con > 0)
		s_strcopy(env->original_env[a], cmd, 0, sizeof_str(cmd, '\0'));
	else
		s_strcopy(env->original_env[a], "  ", 0, 1);
	update_env_value_at_index(env, cmd, a, con);
	env->original_env[a + 1] = 0;
	env->parsed_env[a + 1] = 0;
}
///3

void	replace_env_var(char *var, t_env *env)
{
	int				c;
	int				o;
	char			*env_var;

	c = sizeof_str(var, '=');
	env_var = malloc(c + 1);
	if (!env_var)
		return ;
	s_strcopy(env_var, var, 0, c);
	o = find_env_var_index(env, env_var);
	if (o >= 0)
		remove_env_entry(env, o);
	free(env_var);
	if (c > 0 && c < sizeof_str(var, '\0') - 1)
		add_env_entry(env, var, c, 1);
	else if (var[c] == '=')
		add_env_entry(env, var, c, 0);
	else if (c == sizeof_str(var, '\0'))
		add_env_entry(env, var, c, -1);
}

void	update_env_status(t_env *env, int status, char *start)
{
	char					*var;
	int						a;

	a = count_digits_in_int(status) + sizeof_str(start, '\0') + 1;
	var = malloc(a);
	if (!var)
		return ;
	s_strcopy(var, start, 0, sizeof_str(start, '\0'));
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

int	is_space(char *line)
{
	int				a;

	a = 0;
	while (line[a] == ' ' || line[a] == '\t'
		|| line[a] == '\n')
		a += 1;
	if (line[a] == '\0')
		return (1);
	return (0);
}

int	check_line(char **line)
{
	if (*line[0] == '\0' || str_cmp(*line, "\n", NULL) || is_space(*line))
	{
		free(*line);
		return (1);
	}
	return (0);
}

char	*str_without_char(char *str, char del)
{
	int				a;
	int				b;
	char			*new_str;

	a = 0;
	b = 0;
	new_str = malloc(sizeof_str(str, '\0') + 1);
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
//4

char	*replace_variable_with_value(
		char *old_var, char *__new, int st, int end)
{
	int							size;
	char						*new__;
	int							unsize;

	unsize = sizeof_str(__new, '\0');
	size = st + (sizeof_str(old_var, '\0') - end) + unsize;
	new__ = malloc(size + 1);
	if (!new__)
		return (NULL);
	s_strcopy(new__, old_var, 0, st);
	s_strcopy(new__ + st, __new, 0, unsize);
	s_strcopy(new__ + st + unsize, old_var, end, sizeof_str(old_var, '\0'));
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
	s_strcopy(new_var, var, a + 1, b);
	if (str_cmp(new_var, "?", NULL) && g_var_tn)
		update_env_status(env, g_var_tn, "?=");
	g_var_tn = 0;
	c = find_env_var_index(env, new_var);
	free(new_var);
	*f_hole = 0;
	if (c >= 0)
	{
		new_var = replace_variable_with_value(var, env->parsed_env[c][1], a, b);
		*f_hole = sizeof_str(env->parsed_env[c][1], '\0') + a;
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
	c = count_strings_in_array(args);
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
	free_string_array(args);
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
			head->args[a] = remove_quotes_from_str(head->args[a], 0, 0, 0);
			a++;
		}
	}
	if (head->left)
		expand_variables_in_ast(head->left, env);
	if (head->right)
		expand_variables_in_ast(head->right, env);
}
//5

int	simplified_refactor_thing(char **array, int index, char *str, int in)
{
	int							size;

	if (!str[in] || in > sizeof_str(str, '\0'))
		return (index);
	while (str[in] && str[in] == ' ')
		in++;
	size = sizeof_str(str + in, ' ');
	if (size > sizeof_str(str + in, 34))
		size = sizeof_str(str + in, 34);
	if (size > sizeof_str(str + in, 39))
		size = sizeof_str(str + in, 39);
	if (str[in] == 34)
		size = sizeof_str(str + in + 1, 34) + 2;
	if (str[in] == 39)
		size = sizeof_str(str + in + 1, 39) + 2;
	if ((in + size) > sizeof_str(str, '\0'))
		return (index);
	array[index] = malloc(size + 1);
	if (!array[index])
		return (index);
	s_strcopy(array[index], str, in, in + size);
	return (simplified_refactor_thing(array, index + 1, str, in + size));
}

int	sus_getit_right(char *str, int s_q, int d_q)
{
	int					a;

	a = 0;
	(void)s_q;
	while (str[a])
	{
		if (str[a] == 34 || str[a] == 39
			|| str[a] == '$')
			d_q++;
		a++;
	}
	return (d_q);
}

int	is_flawed_str(char *str, int a, int b, int res)
{
	int					si_q;
	int					do_q;

	si_q = 0;
	do_q = 0;
	while (str[a])
	{
		if (str[a] == 34)
			do_q++;
		else if (str[a] == 39)
			si_q++;
		else if (!(si_q % 2) && !(do_q % 2))
		{
			if (str[a] == ' ')
			{
				if (b)
					res++;
				b = 0;
			}
			else
				b = 1;
		}
		a++;
	}
	return (res);
}

int	detected_flaws(char **array)
{
	int			a;
	int			res;

	res = 0;
	a = 0;
	while (array[a])
	{
		res += is_flawed_str(array[a], 0, 0, 0);
		res += sus_getit_right(array[a], 0, 0);
		a++;
	}
	return (res);
}
/******************************************/
/////PATH HANDLING
// int find_substr_index(char **haystack, char *needle, int n)
// {
//     int a;
//     int b;

//     a = 0;

//     while(haystack[a] != 0)
//     {
//         if(haystack[a][0] == needle[0])
//         {
//             b = 0;
//             while(haystack[a][b]
//              && haystack[a][b] == needle[b])
//              {
//                 if(b == n -1)
//                     return a;
//                 b++;
//              }
//         } 
//         a++;
//     }
//     return -1;
// }

// int is_path_accessible(char *path, int mode)
// {
//     if(access(path, mode) < 0)
//         return 0;
//     return 1;
// }

// void increment_path_index(char *env_v, int *indx, int *a)
// {
//     if(indx[3])
//     {
//         indx[1] += 1;
//         *a = indx[1];
//         while(env_v[indx[1]] != '\0' && env_v[indx[1]] != ':')
//             indx[1] += 1;
//     }else
//         *a = indx[1];
// }

// int count_substrings(char *s, char del)
// {
//     int a;
//     int b;
//     int res;

//     a =0;
//     b = 1;
//     res = 0;

//     while(s && s[a])
//     {
//         if(s[a] != del)
//         {
//             if(b)
//              res += 1;
//             b = 0;
//         }else
//             b = 1;
//         a+=1;
//     }
//     return (res);
// }

// char *create_subpath_from_var(char *env_v, char *f, int *indx)
// {
//     char *tmp_path;
//     int a;
//     int b;
//     int f_size;

//     increment_path_index(env_v, indx, &a);
//     f_size = sizeof_str(f, ' ');
//     tmp_path = malloc((indx[1] - a) + f_size + 2);
//     if(!tmp_path) 
//         return NULL;
//     b = 0;
//     while(b < ((indx[1] - a) + f_size + 1))
//     {
//         if(indx[3] && ( b < indx[1] - a))
//             tmp_path[b] = env_v[a + b];
//         else if(indx[3] && env_v[indx[1] - 1] != '/' && (b == indx[1] - a))
//             tmp_path[b] = '/';
//         else
//             tmp_path[b] = f[b - (indx[1] - a) - indx[3]];
//         b++;
//     }
//     tmp_path[b] = '\0';
//     return(tmp_path);
// }

// char *verify_path_without_env(char *f, int mode)
// {

//     char *tmp_path;
//     int b;

//     b = sizeof_str(f, ' ');
//     tmp_path = malloc(b +1);
//     if(!tmp_path)
//         return NULL;
//     s_strcopy(tmp_path , f, 0, b);
//     if(is_path_accessible(tmp_path, mode))
//         return (tmp_path);
//     free(tmp_path);
//     return NULL;
// }

// char	*fetch_file_path(char *file, char **envp, char *env_var, int mode)
// {
// 	char				*tmp_path;
// 	int					indx_s[4];

// 	indx_s[3] = 0;
// 	indx_s[1] = sizeof_str(env_var, '\0');
// 	indx_s[0] = find_substr_index(envp, env_var, indx_s[1]);
// 	if (indx_s[0] < 0
// 		|| (file[0] && file[1] && file[0] == '.' && file[1] == '/'))
// 		return (verify_path_without_env(file, mode));
// 	indx_s[2] = sizeof_str(envp[indx_s[0]], '\0');
// 	if (sizeof_str(file, ' ') != sizeof_str(file, '\0')
// 		&& !is_path_accessible(file, mode))
// 		return (NULL);
// 	while (envp[indx_s[0]][indx_s[1]])
// 	{
// 		tmp_path = create_subpath_from_var(envp[indx_s[0]], file, indx_s);
// 		if (!tmp_path)
// 			return (NULL);
// 		if (is_path_accessible(tmp_path, mode))
// 			return (tmp_path);
// 		free(tmp_path);
// 		if (!indx_s[3])
// 			indx_s[3] = 1;
// 	}
// 	return (NULL);
// }
size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
		i++;
	return (i);
}
// char	*find_next_substring(char *str, char del, int *index)
// {
// 	char		*sub;
// 	int			size;
// 	int			a;

// 	while (str[index[0]] && str[index[0]] == del)
// 		index[0] += 1;
// 	size = sizeof_str(str + index[0], del);
// 	sub = malloc(size + 1);
// 	if (!sub)
// 		return (NULL);
// 	a = 0;
// 	while (str[index[0]]
// 		&& str[index[0]] != del)
// 	{
// 		if (str[index[0]] != 34 && str[index[0]] != 39)
// 			sub[a++] = str[index[0]];
// 		index[0] += 1;
// 	}
// 	sub[a] = '\0';
// 	index[0] += 1;
// 	return (sub);
// }

char	*ft_strdup(const char *src)
{
	size_t	i;
	size_t	size;
	char	*str;

	i = 0;
	size = ft_strlen(src);
	str = malloc(size + 1);
	if (str == NULL)
		return (NULL);
	while (src[i])
	{
		str[i] = src[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

char	**prepare_cmd_arguments(char *cmd, char **envp, int c)
{
	char		**cmd_arr;
	char		*cmd_holder;
	int			i[3];

	i[1] = count_substrings(cmd, ' ');
	cmd_arr = malloc((i[1] + 1) * sizeof(char *));
	i[0] = 0;
	while (c < i[1])
	{
		cmd_holder = find_next_substring(cmd, '\0', i);
		if (!c && !check_builtin_cmd(cmd_holder))
		{
			cmd_arr[c] = fetch_file_path(cmd_holder, envp, "PATH", X_OK);
			if (!cmd_arr[c])
				cmd_arr[c] = ft_strdup(cmd_holder);
			free(cmd_holder);
		}
		else
			cmd_arr[c] = cmd_holder;
		if (!cmd_arr[c])
			return (NULL);
		c += 1;
	}
	cmd_arr[c] = NULL;
	return (cmd_arr);
}


/////// 
// void init_or_reset_pipe_state(int *piped, int f)
// {
//     int i;

//     i = 0;
//     piped[0]= piped[5];
//     if(f)
//     {
//         while(i < 12)
//         {
//             piped[i++] = 0;
//             //i++;
//         }
//     }
//     else if (piped[5])
//             piped[0]+= 1;
    
//     piped[11] = 1;
// }

void count_red_and_pipes(t_ast_node *head, int *piped)
{
    head->file_type = 0;
    if(head->type == TOKEN_REDIR_OUT || head->type == TOKEN_REDIR_APPEND)
        piped[4] += 1;
    if(head->type == TOKEN_REDIR_IN || head->type == TOKEN_REDIR_HEREDOC)
        piped[3] += 1;
    if(head->type == TOKEN_PIPE)
        piped[5] += 1;
    if(head->left)
        count_red_and_pipes(head->left, piped);
    if(head->right)
        count_red_and_pipes(head->right, piped);
}

void prepare_ast_nodes_to_execute(t_ast_node *head)
{
	if (head->type != TOKEN_WORD)
	{
        if(!head->file_type)
		    head->file_type = FILE_READY;
        // printf("p1\n");
		if (head->type == TOKEN_REDIR_OUT && head->right)
			head->right->file_type = WRITE_FILE;
		if (head->type == TOKEN_REDIR_APPEND && head->right)
			head->right->file_type = WRITE_FILE_APPEND;
		if (head->type == TOKEN_REDIR_IN && head->right)
        {
            // printf("hear\n");
			head->right->file_type = READ_FILE;
        }
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

void	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
}
void	ft_putstr_fd(char *s, int fd)
{
	while (*s != '\0')
	{
		ft_putchar_fd(*s, fd);
		s++;
	}
}


void	ft_putendl_fd(char *s, int fd)
{
	ft_putstr_fd(s, fd);
	ft_putchar_fd('\n', fd);
}


void	handle_ctrl_c(int a)
{
	(void)a;
	g_var_tn = 1;
	// rl_replace_line("", 0);
	write(1, "\n", 1);
	rl_on_new_line();
	rl_redisplay();
}
void	setup_signal_handlers(void)
{
	signal(SIGINT, handle_ctrl_c);
	signal(SIGQUIT, SIG_IGN);
}

void	child_ctrl_c(int sig_num)
{
	if (sig_num == 2)
		g_var_tn = 130;
	else
	{
		ft_putstr_fd("Quit", 1);
		g_var_tn = 131;
	}
	write(1, "\n", 1);
} 

/// 
// int	wait_for_children(int status, int *_piped)
// {
// 	if (status != 2 && status != 127
// 		&& status != 126 && _piped[10]
// 		&& _piped[11])
// 	{
// 		while (_piped[10])
// 		{
// 			wait(&status);
//         //    printf("%d\n",status);
// 			_piped[10] -= 1;
// 		}
// 		signal(SIGINT, handle_ctrl_c);
// 		signal(SIGQUIT, SIG_IGN);
// 		if (!g_var_tn)
// 			return (WEXITSTATUS(status));
// 		else
// 			return (g_var_tn);
// 	}
// 	return (status);
// } 
void	close_pipe_ends(int fd1, int fd2)
{
	close(fd1);
	close(fd2);
}

void	free_string_array(char **arr)
{
	int				a;

	a = 0;
	while (arr[a] != 0)
	{
		free(arr[a]);
		a += 1;
	}
	free(arr);
}
// int execute_basic_command(char **_cmd_, int *_fd, char **env, int *_piped)
// {
// 	pid_t				pid;
// 	int					fd_[2];
//     int status;

// 	pipe(fd_);
// 	pid = fork();
// 	signal(SIGINT, child_ctrl_c);
// 	signal(SIGQUIT, child_ctrl_c);
// 	if (!pid)
// 	{
//        printf("%s\n",_cmd_[0]);
//         //printf("kk");
// 		if (_piped[0] && _piped[0] <= _piped[5])
// 			dup2(_fd[0], 0);
// 		if (_piped[0] > 1)
// 			dup2(fd_[1], 1);
// 		else
// 			close(_fd[0]);
// 		close_pipe_ends(fd_[0], fd_[1]);
// 		execve(_cmd_[0], _cmd_, env);
// 		(ft_putendl_fd(strerror(errno), 2), exit(127));
// 	}
// 	close_pipe_ends(fd_[1], _fd[0]);
// 	if (_piped[0] > 1)
// 		_fd[0] = fd_[0];
// 	else
// 		close(fd_[0]);
   
// //   wait(NULL);    // Wait for child process to finish
// 	return (1);
   
// }

// void	child_fds_managment(int *_piped, int *_fd, int *fd_)
// {
// 	if (_piped[8] && _piped[6])
// 	{
// 		dup2(_piped[1], 0);
// 		close(_piped[1]);
// 	}
// 	if (_piped[8] && _piped[7])
// 	{
// 		dup2(_piped[2], 1);
// 		close(_piped[2]);
// 	}
// 	if (_piped[0] && _piped[0] <= _piped[5]
// 		&& (!_piped[8] || !_piped[6]))
// 		dup2(_fd[0], 0);
// 	if (_piped[0] > 1 && (!_piped[8] || !_piped[7]))
// 		dup2(fd_[1], 1);
// 	else
// 		close(fd_[0]);
// 	close(fd_[0]);
// 	close(fd_[1]);
// }

// void	parent_fds_managment(int *_piped, int *_fd, int *fd_)
// {
// 	if (_piped[8] && _piped[6])
// 	{
// 		close(_piped[1]);
// 		_piped[6] = 0;
// 	}
// 	if (_piped[8] && _piped[7])
// 	{
// 		close(_piped[2]);
// 		_piped[7] = 0;
// 	}
// 	if (!_piped[7] && !_piped[6])
// 		_piped[8] = 0;
// 	close(fd_[1]);
// 	close(_fd[0]);
// 	if (_piped[0] > 1)
// 		_fd[0] = fd_[0];
// 	else
// 		close(fd_[0]);
// }

// int	execute_command_with_redirection(
// 		char **_cmd_, int *_fd, char **env, int *_piped)
// {
// 	pid_t				pid;
// 	int					fd_[2];

// 	pipe(fd_);
// 	pid = fork();
// 	signal(SIGINT, child_ctrl_c);
// 	signal(SIGQUIT, child_ctrl_c);
//       //printf("ee");
// 	if (!pid)
// 	{
//       //printf("a\n");
// 		child_fds_managment(_piped, _fd, fd_);
// 		execve(_cmd_[0], _cmd_, env);
// 		ft_putendl_fd(strerror(errno), 2);
// 		exit(127);
// 	}
// 	parent_fds_managment(_piped, _fd, fd_);
// 	//free_string_array(_cmd_);
// 	return (1);
// }

/////////////////bultin

int is_valid_echo_paran(char *s)
{
    int a;
    a = 0;
    if(s && s[a] && s[a] == '-')
    {
        while(s[++a])
        {
            if(s[a] != 'n')
                return 0;
        }
    }
    else
        return 0;
    return 1;
}
//builtins functions/////

int echo_cmd(char **cmd, int *out_fd)
{
    int a;
    int i;

    i = 0;
    if(cmd[0] && cmd[1] && is_valid_echo_paran(cmd[1]))
        i = 1;
    a = i + 1;
    while(i && cmd[a] && is_valid_echo_paran(cmd[a]))
        a++;
    if((cmd[0] && cmd[a]) || sizeof_str(cmd[a], '\0'))
    {
        while(1)
        {
            ft_putstr_fd(cmd[a], out_fd[1]);
            a++;
            if(cmd[a])
                write(out_fd[1], "", 1);
            else
                break;

        }
    }
    if(!i)
        write(out_fd[1], "\n", 1);
    return 0;
}

int change_current_directory(char *path, t_env *env)
{
    int status;
    int a;

    if(!path || !sizeof(path, '\0'))
    {
        a =find_env_var_index(env, "HOME");
        if(a >=0)
            status = chdir(env->parsed_env[a][1]);
        else
            status = -1;
    }
    else
        status = chdir(path);
    return status;
}

char *get_current_working_directory(int size, int tries, int fd)
{
    char *buff;
    buff = malloc(size);
    if(!buff)
     return NULL;
    if(!getcwd(buff, size))
    {
        free(buff);
        if(tries < 10)
         return (get_current_working_directory(size +50, tries +1, fd));
        else
        {
            ft_putendl_fd("  err:cd() / pwd(): getcwd():you are lost", fd);
            return NULL;
        }
    }
    return buff;
}
int export_print_or_export(char **cmd)
{
    int a;
    a = 1;
    while(cmd[0] && cmd[a])
    {
        if(cmd[a])
            return 1;
        a++;
    }
    return 0;
}
int export_statment_check(char *cmd)
{
    int a;
    int b;

    a = sizeof_str(cmd, '=');
    if(a > 1 && cmd[a - 1] == '+')
         a-=1;
    if(a)
    {
        b = 0;
        while(b < a)
        {
            if(!b && !ft_isalpha(cmd[b]) && cmd[b] != '-')
                return -1;
            else if( b && !ft_isalnum(cmd[b]) && cmd[b] != '-')
                return  -1;
            b++;
        }
    }
    return a;
}

char **export_cmd(char **cmd, t_env *env, int *out_fd, int **s)
{
    int a;
    int b;

    a = 1;
    while(cmd[a])
    {
        b = export_statment_check(cmd[a]);
        if(b > 0)
        {
            if(b >= 1 && cmd[a][b] == '+')
                append_env_var(cmd[a], env);
            else
                replace_env_var(cmd[a], env);
        }
        else
        {
            ft_putendl_fd(" err: export(\'", out_fd[1]);
            ft_putstr_fd(cmd[a], out_fd[1]);
            ft_putendl_fd("\' : Not a valid thing", out_fd[1]);
            **s = 256;
        }
        a++;
    }
    return cmd;
}
int	string_weight_compare(char *s_1, char *s_2)
{
	int					a;

	a = 0;
	while (s_1[a] && s_2[a])
	{
		if (s_1[a] > s_2[a])
			return (0);
		else if (s_2[a] > s_1[a])
			return (1);
		a++;
	}
	if (s_2[a])
		return (1);
	else if (s_1[a])
		return (0);
	return (0);
}

int	check_array_arrangment(char ***array, int _si)
{
	int					a;
	int					b;
	int					c;

	a = 0;
	while (a < _si - 1)
	{
		b = a + 1;
		while (b < _si)
		{
			c = string_weight_compare(array[a][0], array[b][0]);
			if (c || (c < 0 && (b - a) > 1))
				return (0);
			b++;
		}
		a++;
	}
	return (1);
}
char	***sort_tha_array(char ***_array, int _si)
{
	int					a;
	int					b;
	int					c;
	char				**temp_p;

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
void	print_export_vars(char ***arr, int a, int fd)
{
	while (a--)
	{
		if (!str_cmp(arr[a][0], "?", NULL))
		{
			ft_putstr_fd("declare -x ", fd);
			ft_putstr_fd(arr[a][0], fd);
			if (!str_cmp(arr[a][1], "F1", NULL))
			{
				write(fd, "=", 1);
				ft_putchar_fd('"', fd);
				if (arr[a][1][0])
					ft_putstr_fd(arr[a][1], fd);
				ft_putchar_fd('"', fd);
			}
			write(fd, "\n", 1);
		}
	}
}
void print_export_declaration_to_fd(t_env *env, int *out_fd)
{
	char ***new_arr;
	int a;
	a = 0;
	while(env->parsed_env[a] != 0)
		a++;
	if(!a)
		return ;
	new_arr = duplicate_env_structure(env, a, 'F', -1);
	new_arr[a] = 0;
	new_arr= sort_tha_array(new_arr, a);
	print_export_vars(new_arr, a, out_fd[1]);
	free_environment_variables(new_arr);
}

void	print_env_var_to_fd(char *str_1, char *str_2, int fd)
{
	if (!str_cmp(str_1, "?", NULL)
		&& !str_cmp(str_2, "F1", NULL))
	{
		ft_putstr_fd(str_1, fd);
		ft_putstr_fd("=", fd);
		if (str_2[0])
			ft_putstr_fd(str_2, fd);
		write(fd, "\n", 1);
	}
}

int env_or_pwd_cmd(char *cmd, t_env *env, int c, int *out_fd)
{
	int a;
	char *abs_pwd;

	a = -1;

	if(str_cmp(cmd, "env", NULL))
	{
		if(c)
			print_export_declaration_to_fd(env, out_fd);
		else{
			while(env->parsed_env[++a])
				print_env_var_to_fd(env->parsed_env[a][0], env->parsed_env[a][1], out_fd[1]);
			
		}
		return 0;	
	}
	abs_pwd = get_current_working_directory(100, 5,out_fd[1]);
	if(abs_pwd)
	{
		ft_putendl_fd(abs_pwd, out_fd[1]);
		return (free(abs_pwd), 0);
	}
	return 256;
}
char **unset_or_export_cmd(char **cmd, t_env *env, int *out_fd, int *s)
{
    int a;
    int c;
    a =1;
    *s = 0;
	
		
    if(cmd[0] && str_cmp(cmd[0], "unset", NULL) && env->parsed_env[0])
    {
		//write(out_fd[1],"test\n",5);
		write(out_fd[1],env->parsed_env[30][0],2);
        while(cmd[a])
        {
            c= find_env_var_index(env, cmd[a]);
				write(out_fd[1],"test\n",5);
			
            if(c >= 0)
			{
				write(out_fd[1],"test2\n",6);
                remove_env_entry(env, c);
			}
            else 
                *s =256;
            a++;
        }
    }
    else if(str_cmp(cmd[0], "export", NULL))
    {
        if(export_print_or_export(cmd))
            cmd = export_cmd(cmd, env, out_fd, &s);
        else
            env_or_pwd_cmd("env", env, 1, out_fd);
    }
	return(cmd);
}
void set_new_pwd_in_env(char *new, t_env *env, int c, int *out_fd)
{

    char **exp_pwd;
    int a;
    int b;

    a =0;
    b = 4;
  //  void(c);
    exp_pwd = malloc(3 * sizeof(char *));
    if(!exp_pwd)
        return ;
    exp_pwd[0] = strcopy("export");
    exp_pwd[1] = malloc(sizeof_str(new, '\0') + 5);
    if(!exp_pwd[1])
     return ;
    s_strcopy(exp_pwd[1], "PWD=",0,4);
	//printf("kk");
    while(new[a])
        exp_pwd[1][b++] = new[a++];
    exp_pwd[1][b]= '\0';
    exp_pwd[2] = 0;
    unset_or_export_cmd(exp_pwd, env, NULL, &a);
//	write(out_fd[1],"test\n",5);
	free_string_array(exp_pwd);
}
int cd_cmd(char **cmd, t_env *env, int *out_fd)
{
    int a;
    char *new_path;

    if(cmd[1] && cmd[2])
        ft_putendl_fd("err: cd(): Not a cd thing", out_fd[1]);
    else{
        if(change_current_directory(cmd[1], env) < 0)
            ft_putendl_fd("err:cd():Only EXISTING destination", out_fd[1]);
        else{
            a = find_env_var_index(env,"PWD");
            if(a >= 0)
                remove_env_entry(env, a);
            new_path = get_current_working_directory(100, 5, out_fd[1]);
            if(new_path)
            {
                set_new_pwd_in_env(new_path,env,a, out_fd);
			//write(out_fd[1],"test\n",5);
				free(new_path);
            }
			return 0;
        }
    }
	return 256;
}



// int builtin_cmd_exec_in_child(char **cmd, t_env *env, int *out_fd, int *piped)
// {
//     int status;

//     status = 0;
//     (void)piped;
//     if(str_cmp(cmd[0], "echo", NULL))
//         status = echo_cmd(cmd, out_fd);
// 	else if(str_cmp(cmd[0], "pwd", "env"))
// 		status = env_or_pwd_cmd(cmd[0], env, 0, out_fd);
// 	else if (str_cmp(cmd[0], "export", "unset"))
// 		cmd = unset_or_export_cmd(cmd,env,out_fd, &status);
// 	else if(str_cmp(cmd[0], "cd", NULL))
// 		status = cd_cmd(cmd,env,out_fd);
//     else
//         write(out_fd[1], "no\n", 3);
//    free_string_array(cmd);
//     return (status);
// }

// int simple_child_in_builtin(char **cmd,int *fd, t_env *env, int *piped)
// {
//     pid_t pid;
//     int pfd[2];
//     int out_fd[2];
//     int status;
//     (pipe(pfd), pid = fork());
//     if(!pid)
//     {
//         if(piped[0] && piped[0] <= piped[5])
//         {
//             dup2(fd[0], 0);
//         }
//         if(piped[0 > 1])
//             dup2(pfd[1], 1);
//         else    
//             close(fd[0]);
//         close_pipe_ends(pfd[0], pfd[1]);
//         dup2(1, out_fd[1]);
//         status=builtin_cmd_exec_in_child(cmd, env, out_fd, piped);
//         exit(WEXITSTATUS(status));
//     }
//     close_pipe_ends(pfd[1], fd[0]);
//     if(piped[0] > 1)
//         fd[0] = pfd[0];
//     else
//         close(pfd[0]);
//     return 1;
// }
// int	execute_builtin_with_piping(char **_cmd_, int *_fd, t_env *env, int *_piped)
// {
// 	int				status;
// 	int				_out_fd[2];

// 	_out_fd[1] = 1;
// 	if (_piped[0] > 1)
// 		pipe(_out_fd);
//    // write(_out_fd[1], "tt\n", 3);
// 	status = builtin_cmd_exec_in_child(_cmd_, env, _out_fd, _piped);
// 	if (_piped[0] > 1)
// 	{
// 		close(_out_fd[1]);
// 		_fd[0] = _out_fd[0];
// 	}
// 	return (status);
// }

// int builtin_with_simple_piping_exec(char **cmd, int *fd,t_env *env , int *piped)
// {
//     int status;
//     int fd_out[2];

//     fd_out[1] = 1;

//     if(piped[8] && piped[6])
//         fd_out[1] = piped[2];
//     if(piped[0] > 1 && (!piped[8] || ! piped[7]))
//         pipe(fd_out);
//     status = builtin_cmd_exec_in_child(cmd, env, fd_out, piped);
//     if(piped[7] && piped[8])
//     {
//         close(fd_out[1]);
//         piped[7] = 0;
//     }
//     if(!piped[6] && !piped[7])
//         piped[8] = 0;
//     if(piped[0] > 1 && (!piped[8] || !piped[7]))
//     {
//         close(fd_out[1]);
//         fd[0] = fd_out[0];
//     }
//     return status;
// }

// void builtin_exec_and_exit(char **cmd, t_env *env, int *out_fd, int *piped)
// {
//     int status;
//     status = builtin_cmd_exec_in_child(cmd, env, out_fd, piped);
//     exit(WEXITSTATUS(status));
// }

// int child_redirection(char **cmd, int *fd, t_env *env, int *piped)
// {
//     pid_t pid;
//     int out_fd[2];

//     out_fd[1] = 1;
//      write(out_fd[1], "ss\n", 3);
//     if(piped[8] && piped[7])
//     {
       
//         out_fd[1] = piped[2];
//     }
//     if(piped[0] > 1 && (!piped[8] || !piped[7]))
//         pipe(out_fd);
//     pid = fork();
//     if(!pid)
//         builtin_exec_and_exit(cmd, env, out_fd, piped);
//     if(piped[8] && piped[7])
//     {
//         close(out_fd[1]);
//         piped[7] = 0;
//     }
//     if(!piped[6] && !piped[7])
//         piped[8] = 0;
//     if(piped[0] > 1 && (!piped[7] || !piped[8]))
//     {
//         close(out_fd[1]);
//         fd[0] = out_fd[0];
//     }
//     return 1;
// }

// int simple_builtin_execution_managment(char **cmd, int * fd,t_env *env, int *piped)
// {
//     int status;
//     int exec_status;

//     status = 0;
//     if(str_cmp(cmd[0], "exit", NULL))
//     {
//         //here
//     }
//     else if(!piped[8])
//         status= execute_builtin_with_piping(cmd, fd, env, piped);
//     else
//         status = builtin_with_simple_piping_exec(cmd,fd,env, piped);
    
//     return status;

// }

// int manage_bultin_exec(char **cmd, int *fd, t_env *env, int *piped)
// {
//     int status;

//     status = 0;
//     piped[10] += 1;
//     printf("%s\n", cmd[0]);
//     //piped[0] = 1;
//     if(piped[0])
//     {
//             printf("aa");
//         if(!piped[8])
//         {
//             status = simple_child_in_builtin(cmd, fd, env, piped);
//         }   
//         // else
//         // {
//         //     printf("uu\n");
//         //     status = child_redirection(cmd, fd, env, piped);
//         // }

//        free_string_array(cmd);
//     }
//     else if(piped[4] || piped[3])
//     {
//          status = child_redirection(cmd, fd, env, piped);
//     }
//     else
//     {
//         printf("ee\n");
//         status = simple_builtin_execution_managment(cmd, fd, env, piped);
//     }
//     return status;
// }
// int check_builtin_cmd(char *cmd)
// {
//     char *tmp_cmd;
//     int status;

//     status = 0;
//     tmp_cmd = malloc(sizeof_str(cmd, ' ') + 1);
//     if(!tmp_cmd)
//         return 0;
//     s_strcopy(tmp_cmd, cmd,0, sizeof_str(cmd, ' '));
//     if(str_cmp(tmp_cmd, "echo","cd")
//         || str_cmp(tmp_cmd, "pwd","export")
//         || str_cmp(tmp_cmd, "unset","env")
//         || str_cmp(tmp_cmd, "exit",NULL))
//         status = 1;
//     free(tmp_cmd);
//     return status;
// }
// char	**merge_command_args(char **f_args, char **_cmd_)
// {
// 	int				a;
// 	char			**new_args;

// 	if (!f_args)
// 		return (NULL);
// 	a = 1;
// 	while (_cmd_[0] && _cmd_[a])
// 		a++;
// 	new_args = malloc((a + 1) * sizeof(char *));
// 	if (!new_args)
// 		return (NULL);
// 	a = 0;
// 	if (f_args[a])
// 		new_args[a] = strcopy(f_args[a]);
// 	else
// 		new_args[a] = strcopy("");
// 	if (_cmd_ && _cmd_[0])
// 	{
// 		while (_cmd_[++a])
// 			new_args[a] = strcopy(_cmd_[a]);
// 	}
// 	new_args[a] = 0;
// 	free_string_array(f_args);
// 	return (new_args);
// }

// int preparation_to_exexcute_command(char **cmd, int *fd, int *piped, t_env *env)
// {
//    char **cmd_args;
//     char **f_args;
//     int status;
//    // cmd_args = cmd;

// f_args = prepare_cmd_arguments(cmd[0], env->original_env, 0);
// cmd_args = merge_command_args(f_args, cmd);
//     if(check_builtin_cmd(cmd_args[0]))
//     {
//         printf("%d\n", piped[0]);
//         status = manage_bultin_exec(cmd_args, fd, env, piped);
//     }
//     else{
//           piped[10] += 1;
//         //  printf("%d\n", piped[10]);
//         if(!piped[8])
//         {
//             status = execute_basic_command(cmd_args, fd, env->original_env,piped);
//             printf("%d\n", status);
//             free_string_array(cmd_args);
//         }else
//         {
//         //  printf("f3\n");
//             status = execute_command_with_redirection( cmd_args, fd,env->original_env, piped);
//         }
//     }
  
//     if(piped[0] > 1)
//         piped[0] -= 1;
//     return (status);
// }
int manage_piped_execution(t_ast_node * head,int *piped ,t_env *env, int *fd)
{
    int status;
// printf("aa\n");
    if(head->file_type == EXECUTE_FILE)
    {
        piped[8] = 0;
        status = preparation_to_exexcute_command(head->args,fd, piped,env);
       // printf("%d\n", status);
    }
    if (head->type == TOKEN_REDIR_IN
		|| head->type == TOKEN_REDIR_OUT
		|| head->type == TOKEN_REDIR_APPEND
		|| head->type == TOKEN_REDIR_HEREDOC)
        {
         //  return(handle_redirections_cmd( head, piped, env, fd));
        }
    if(head->left)
        status = manage_piped_execution(head->left,piped, env, fd);
    if(head->right)
        status =  manage_piped_execution(head->right,piped, env, fd);
   //printf("f2\n");
    return (status);
}

// int ast_node_execution(t_ast_node *head, int *piped, t_env *env)
// {
//     int fd[2];
//     int status;

//     fd[0] = -1;
//     fd[1] = -1;

//     if(head->file_type == FILE_READY)
//     {
//         if(head->type == TOKEN_PIPE)
//             status = manage_piped_execution(head, piped, env, fd);
//         if (head->type == TOKEN_REDIR_IN
// 			|| head->type == TOKEN_REDIR_OUT
// 			|| head->type == TOKEN_REDIR_APPEND
// 			|| head->type == TOKEN_REDIR_HEREDOC)
//             {

//               // printf("jjj");
//             status= handle_redirections_cmd(head, piped, env, fd);
//             }
//     }
//     if(head->file_type == EXECUTE_FILE)
//     {
//     //printf("aa\n");

//         status = preparation_to_exexcute_command(head->args, fd, piped, env);
//         //printf("%d\n", status);
//     }
//     status = wait_for_children(status, piped);
 
//    // wait(NULL);
//     if (piped[6])
// 		close(piped[1]);
// 	if (piped[7])
// 		close(piped[2]);
//     if(fd[0] != -1 || fd[1] != -1)
//         (close(fd[0]), close(fd[1]));
//     return(g_var_tn =0, status);
// }


///redirctions
// int switch_fds_id(
// 		int *_piped, int index, int index_2, int con)
// {
// 	if (con)
// 	{
// 		if (_piped[index])
// 			close(_piped[index_2]);
// 		_piped[index] = 1;
// 	}
// 	else
// 	{
// 		ft_putendl_fd("err: file not found", 2);
// 		_piped[6] = 0;
// 	}
// 	return (1);
// }

///////////////////////////////
//herdoc_handling

//string function
char	*remove_quotes_from_str(char *str, int si_q_c, int do_q_c, int a)
{
	char				*new_str;
	int					b;

	b = 0;
	new_str = malloc(sizeof_str(str, '\0') + 1);
	while (str[a])
	{
		if (str[a] == 34 && !(si_q_c % 2))
			do_q_c++;
		else if (str[a] == 39 && !(do_q_c % 2))
			si_q_c++;
		if ((str[a] != 34 || si_q_c % 2)
			&& ((str[a] != 39) || do_q_c % 2))
			new_str[b++] = str[a];
		a++;
	}
	new_str[b] = '\0';
	return (free(str), new_str);
}
///
// int str_comper(char *s1, char *s2, int max)
// {
//     int a;
//     a = 0;
//     while(a < max && *s1 && *s2 && s1[a] == s2[a])
//         a++;
//     if(sizeof_str(s1, '\0') == a && a == max)
//         return 1;
//     return 0;
// }

// void exite_herdoc(int a)
// {
//     (void) a;
//     write(1, "\n", 1);
//     exit(0);
// }

// int  is_there_quotes(char *s)
// {
//     int a;
//     a = 0;

//     while(a && s[a])
//     {
//         if(s[a] == 34 || s[a] == 39)
//             return 0;
//         a++;
//     }
//     return 1;

// }

// void read_and_write(int std_out, char *dlmtr, t_env *env, int is_expanded)
// {
//     char *buff;
//     int f_arr[3];

//     dlmtr = remove_quotes_from_str(dlmtr, 0,0,0);
//     while(1)
//     {
//         buff = readline(">> ");
//         if(!buff || str_comper(dlmtr, buff, sizeof_str(buff, '\n')))
//         {
//            //write(std_out, "a\n", 2);
//             free(buff);
//             break;
//         }
       
//        // write(std_out, "b\n", 2);
//         write(std_out, buff, sizeof(buff, '\n'));
//         write(std_out, "\n", 1);
//         free(buff);
//     }
//     free(dlmtr);
// }

// int here_doc_execution(char *delmtr, int * piped, t_env *env)
// {
//     int out_fd[2];
//     pid_t pid;
//     int status;


//     pipe(out_fd);
//     pid = fork();
//     signal(SIGINT, SIG_IGN);
//    // printf("%s\n", delmtr);
//     if(!pid)
//     {
//         //printf("ss\n");
//         signal(SIGINT, exite_herdoc);
//         close(out_fd[0]);
//         read_and_write(out_fd[1], delmtr,env,  is_there_quotes(delmtr));
//         //  write(out_fd[1], delmtr, sizeof(delmtr));
//         exit(1);
//     }
//     waitpid(pid, &status, 0);
//     close(out_fd[1]);
//     piped[1] = out_fd[0];
//     piped[9] = (WEXITSTATUS(status)) - 1;
//     if(piped[9] < 0)
//         piped[9] +=2;
//     piped[11] = status;
//     return (piped[9]);
// }
// int file_to_redirection_open(
// 		t_ast_node *head, int *_piped, t_env *env, int status)
// {
// 	int			mode;

// 	if (head->file_type == READ_FILE)
// 	{
//         printf("hh\n");
// 		switch_fds_id(_piped, 6, 1, 1);
// 		_piped[1] = open(head->args[0], O_RDONLY);
// 		if (_piped[1] < 0)
// 			status = switch_fds_id(_piped, 0, 0, 0);
// 	}
// 	else if (head->file_type == READ_FROM_APPEND)
// 	{
// 		switch_fds_id(_piped, 6, 1, 1);
// 		status = here_doc_execution(head->args[0], _piped, env);
// 		signal(SIGINT, handle_ctrl_c);
//         //printf("gg");
// 	}
// 	else
// 	{
//         printf("ff\n");
// 		switch_fds_id(_piped, 7, 2, 1);
// 		mode = O_TRUNC;
// 		if (head->file_type == WRITE_FILE_APPEND)
// 			mode = O_APPEND;
// 		_piped[2] = open(head->args[0], O_WRONLY | O_CREAT | mode, 0666);
// 	}
// 	return (status);
// }

/////handle redirecrtiona commans



int handle_redirections_cmd(t_ast_node *head, int *piped, t_env *env, int *fd)
{
   int status;
       // printf("aaa");

   piped[11] = 1;
   if(head->right)
   {
    status = file_to_redirection_open(head->right, piped, env, 0);
    if((status || !head->left) && piped[0] > 1)
        piped[0] -= 1;
   }
   if(head->left && head->left->file_type == EXECUTE_FILE
        && piped[11] && !status)
    {
        piped[8] = 1;
        status = preparation_to_exexcute_command(head->left->args, fd, piped,env);
    }
    if(head->left && head->left->file_type == TOKEN_PIPE && piped[11])
        status = manage_piped_execution(head->left, piped, env, fd);
    if(head->left && (head->left->type == TOKEN_REDIR_IN
                        || head->left->type == TOKEN_REDIR_OUT
                        || head->left->type == TOKEN_REDIR_APPEND
                        || head->left->type == TOKEN_REDIR_HEREDOC))
                    status = handle_redirections_cmd(head->left, piped, env, fd);
    return(status);
}



////////////////
// void manage_execution_commands(t_ast_node *head, t_env *env)
// {
//     int piped[13];
//     int status;
   

//       //int fd[2] = { -1, -1 };
//     init_or_reset_pipe_state(piped, 1);
//     count_red_and_pipes(head, piped);
//     init_or_reset_pipe_state(piped,0);
//     prepare_ast_nodes_to_execute(head);
//     expand_variables_in_ast(head, env);
//     // display_ast(head, 0);
//     printf("%d\n",piped[4]);
//    //piped[10] += 1;
//    ast_node_execution(head,piped,env);
// //    wait_for_children(status,piped);
//    //wait(NULL);
// }

/////////////////////////////////////////////////////////////////////////////////////
///
// Function to map token types to string (for printing)
// Function to create a new AST node
// t_ast_node *create_node(t_token_type type, char **args) {
//     t_ast_node *node = malloc(sizeof(t_ast_node));
//     if (!node) {
//         perror("Failed to allocate memory for AST node");
//         exit(EXIT_FAILURE);
//     }
//     node->type = type;
//     node->args = args;   // Assuming args is already allocated
//     node->left = NULL;
//     node->right = NULL;
//     return node;
// }
// // Function to duplicate command arguments
// char **duplicate_args(char **args) {
//     if (!args) return NULL;

//     int count = 0;
//     while (args[count]) count++;

//     char **new_args = malloc((count + 1) * sizeof(char *));
//     if (!new_args) {
//         perror("Failed to allocate memory for arguments");
//         exit(EXIT_FAILURE);
//     }

//     for (int i = 0; i < count; i++) {
//         new_args[i] = strdup(args[i]); // Duplicate each argument
//     }
//     new_args[count] = NULL; // Null-terminate the array
//     return new_args;
// }

// // Example function to create the specific pipeline command tree
// t_ast_node *create_pipeline() {
//     // Create the leaves (commands)
//     char *cat_args[] = {"/bin/cat", "text.txt", NULL};
//     char *grep_args[] = {"/usr/bin/grep", "text", NULL};
//     char *sort_args[] = {"/usr/bin/sort", NULL};

//     t_ast_node *cat_node = create_node(TOKEN_WORD, duplicate_args(cat_args));
//     t_ast_node *grep_node = create_node(TOKEN_WORD, duplicate_args(grep_args));
//     t_ast_node *sort_node = create_node(TOKEN_WORD, duplicate_args(sort_args));

//     // Create the first pipe node (cat | grep)
//     t_ast_node *pipe_node_1 = create_node(TOKEN_PIPE, NULL);
//     pipe_node_1->left = cat_node;
//     pipe_node_1->right = grep_node;

//     // Create the second pipe node ((cat | grep) | sort)
//     t_ast_node *pipe_node_2 = create_node(TOKEN_PIPE, NULL);
//     pipe_node_2->left = pipe_node_1;
//     pipe_node_2->right = sort_node;

//     return pipe_node_2; // Return the root of the pipeline
// }

// // Function to display the AST (for testing)
// void display_ast(t_ast_node *node, int level) {
//     if (!node) return;
//     for (int i = 0; i < level; i++) {
//         printf("  ");
//     }
//     if (node->type == TOKEN_PIPE) {
//         printf("[PIPE]\n");
//     } else {
//         printf("[COMMAND]: %s\n", node->args[0]);
//     }
//     display_ast(node->left, level + 1);
//     display_ast(node->right, level + 1);
// }

// // Function to free the AST (for memory management)
// void free_ast(t_ast_node *node) {
//     if (!node) return;
//     free_ast(node->left);
//     free_ast(node->right);
//     free(node->args);  // Free argument list
//     free(node);        // Free the node
// }

// Function to create a new AST node
t_ast_node *new_ast_node(t_token_type type) {
    t_ast_node *node = (t_ast_node *)malloc(sizeof(t_ast_node));
    if (!node) {
        perror("Failed to allocate memory for AST node");
        exit(EXIT_FAILURE);
    }
    node->type = type;
    node->args = NULL;
    node->left = NULL;
    node->right = NULL;
    return node;
}

// Function to create a command node
t_ast_node *create_command_node(char *command) {
    t_ast_node *node = new_ast_node(TOKEN_WORD);
    node->args = malloc(sizeof(char *) * 3);
    node->args[0] = strdup(command);
//    node->args[1] = strdup(arg);
//    node->args[2] = strdup(arg1);
    node->args[1] = NULL;
    return node;
}

// Function to create a file node (for redirection)
t_ast_node *create_file_node(t_token_type type, char *filename) {
    t_ast_node *node = new_ast_node(type);
    node->args = malloc(sizeof(char *) * 2);
    node->args[0] = strdup(filename);
    node->args[1] = NULL;
    return node;
}

// Function to create the AST for the command "cat < input.txt"
// t_ast_node *create_ast_for_command() {
//     // Create the command node for "cat"
//     t_ast_node *command_node = create_command_node("/bin/cat");

//     // Create the input redirection node (< input.txt)
//     t_ast_node *input_node = new_ast_node(TOKEN_REDIR_IN);
//     input_node->left = command_node; // Link "cat" to the input redirection
//     input_node->right = create_file_node(TOKEN_REDIR_IN, "input.txt");

//     // The root node of the AST is the input redirection node
//     return input_node;
// }

t_ast_node *create_ast_for_command() {
    // Create the command node for "cat"
    t_ast_node *command_node = create_command_node("pwd");

    // Create the input redirection node (< input.txt)
    t_ast_node *input_node = new_ast_node(TOKEN_REDIR_OUT);
    input_node->left = command_node; // Link "cat" to the input redirection
    input_node->right = create_file_node(TOKEN_REDIR_OUT, "ll");

    // Create the output redirection node (> output.txt)
    // t_ast_node *output_node = new_ast_node(TOKEN_REDIR_OUT);
    // output_node->left = input_node; // Link input redirection to the output redirection
    // output_node->right = create_file_node(TOKEN_REDIR_OUT, "output.txt");

    // The root node of the AST is the output redirection node
    return command_node;
}

// Function to print the AST (for testing purposes)
void print_ast(t_ast_node *node, int level) {
    if (!node) return; // Check if the node is NULL

    // Indentation for readability
    for (int i = 0; i < level; i++) {
        printf("  ");
    }

    // Check node type and print accordingly
    if (node->type == TOKEN_WORD && node->args) {
        printf("[COMMAND]: %s,,,, : %d\n", node->args[0], node->file_type);
    } else if (node->type == TOKEN_REDIR_OUT && node->args) {
        printf("[INPUT REDIRECTION]: %s: %d\n", node->args[0], node->file_type);
    } else {
        printf("[UNKNOWN NODE],%d \n", node->file_type);
    }

    // Recursively print left and right children, if they exist
    if (node->left) {
        printf("2\n");
        print_ast(node->left, level + 1);
    }
    if (node->right) {
        printf("3\n");
        print_ast(node->right, level + 1);
    }
}
int main(int argc, char **argv, char **original_env)
{
    t_env *env;
    int status;
    

    (void)argv;

    env = malloc(sizeof(t_env));
    status = shell_initializing_with_env(env, original_env);
    t_ast_node *pipeline = create_ast_for_command();
    // Command and arguments for `cat text.txt`
    // char **args = malloc(3 * sizeof(char *));
    // args[0] = strdup("/bin/cat");
    // args[1] = strdup("text.txt");
    // args[2] = NULL;  // NULL-terminated array of arguments

    // // Create the AST node for this command
    // t_ast_node *node = create_ast_node(TOKEN_WORD, args);
    // //node->type = TOKEN_WORD;
    // // // Free the allocated memory
	// int c= find_env_var_index(env, pipeline->args[1]);
    //         if(c >= 0)
	// 		{
	// 			write(1,"test\n",5);
    //             remove_env_entry(env, c);
	// 		}
	int i = 0;
while(env->original_env[i])
{
	printf("'%s'\n", env->parsed_env[i][0]);
	i++;
}
    
    if(status)
    {
       //prepare_ast_nodes_to_execute(pipeline);
      manage_execution_commands(pipeline, env);
       print_ast(pipeline,0);
       //display_ast(pipeline, 0);
        // int a =0;
        // while(env->original_env[a])
        // {
        //     printf("%s\n", env->original_env[a]);
        //     a++;
        // }
        
    }
    // free_ast(pipeline);

     // Clean up
    // free(args[0]);
    // free(args[1]);
    // free(args);
    // free(node);
    return 0;

}
