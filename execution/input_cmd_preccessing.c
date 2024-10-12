

char	**merge_command_args(char **f_args, char **_cmd_)
{
	int				a;
	char			**new_args;

	if (!f_args)
		return (NULL);
	a = 1;
	while (_cmd_[0] && _cmd_[a])
		a++;
	new_args = malloc((a + 1) * sizeof(char *));
	if (!new_args)
		return (NULL);
	a = 0;
	if (f_args[a])
		new_args[a] = strcopy(f_args[a]);
	else
		new_args[a] = strcopy("");
	if (_cmd_ && _cmd_[0])
	{
		while (_cmd_[++a])
			new_args[a] = strcopy(_cmd_[a]);
	}
	new_args[a] = 0;
	free_string_array(f_args);
	return (new_args);
}