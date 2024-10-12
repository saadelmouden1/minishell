


char *create_subpath_from_var(char *env_v, char *f, int *indx)
{
    char *tmp_path;
    int a;
    int b;
    int f_size;

    increment_path_index(env_v, indx, &a);
    f_size = sizeof_str(f, ' ');
    tmp_path = malloc((indx[1] - a) + f_size + 2);
    if(!tmp_path) 
        return NULL;
    b = 0;
    while(b < ((indx[1] - a) + f_size + 1))
    {
        if(indx[3] && ( b < indx[1] - a))
            tmp_path[b] = env_v[a + b];
        else if(indx[3] && env_v[indx[1] - 1] != '/' && (b == indx[1] - a))
            tmp_path[b] = '/';
        else
            tmp_path[b] = f[b - (indx[1] - a) - indx[3]];
        b++;
    }
    tmp_path[b] = '\0';
    return(tmp_path);
}

char *verify_path_without_env(char *f, int mode)
{

    char *tmp_path;
    int b;

    b = sizeof_str(f, ' ');
    tmp_path = malloc(b +1);
    if(!tmp_path)
        return NULL;
    s_strcopy(tmp_path , f, 0, b);
    if(is_path_accessible(tmp_path, mode))
        return (tmp_path);
    free(tmp_path);
    return NULL;
}

char	*fetch_file_path(char *file, char **envp, char *env_var, int mode)
{
	char				*tmp_path;
	int					indx_s[4];

	indx_s[3] = 0;
	indx_s[1] = sizeof_str(env_var, '\0');
	indx_s[0] = find_substr_index(envp, env_var, indx_s[1]);
	if (indx_s[0] < 0
		|| (file[0] && file[1] && file[0] == '.' && file[1] == '/'))
		return (verify_path_without_env(file, mode));
	indx_s[2] = sizeof_str(envp[indx_s[0]], '\0');
	if (sizeof_str(file, ' ') != sizeof_str(file, '\0')
		&& !is_path_accessible(file, mode))
		return (NULL);
	while (envp[indx_s[0]][indx_s[1]])
	{
		tmp_path = create_subpath_from_var(envp[indx_s[0]], file, indx_s);
		if (!tmp_path)
			return (NULL);
		if (is_path_accessible(tmp_path, mode))
			return (tmp_path);
		free(tmp_path);
		if (!indx_s[3])
			indx_s[3] = 1;
	}
	return (NULL);
}

char	*find_next_substring(char *str, char del, int *index)
{
	char		*sub;
	int			size;
	int			a;

	while (str[index[0]] && str[index[0]] == del)
		index[0] += 1;
	size = sizeof_str(str + index[0], del);
	sub = malloc(size + 1);
	if (!sub)
		return (NULL);
	a = 0;
	while (str[index[0]]
		&& str[index[0]] != del)
	{
		if (str[index[0]] != 34 && str[index[0]] != 39)
			sub[a++] = str[index[0]];
		index[0] += 1;
	}
	sub[a] = '\0';
	index[0] += 1;
	return (sub);
}