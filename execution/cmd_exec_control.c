

void init_or_reset_pipe_state(int *piped, int f)
{
    int i;

    i = 0;
    piped[0]= piped[5];
    if(f)
    {
        while(i < 12)
        {
            piped[i++] = 0;
            //i++;
        }
    }
    else if (piped[5])
            piped[0]+= 1;
    
    piped[11] = 1;
}

int switch_fds_id(
		int *_piped, int index, int index_2, int con)
{
	if (con)
	{
		if (_piped[index])
			close(_piped[index_2]);
		_piped[index] = 1;
	}
	else
	{
		ft_putendl_fd("err: file not found", 2);
		_piped[6] = 0;
	}
	return (1);
}

int file_to_redirection_open(
		t_ast_node *head, int *_piped, t_env *env, int status)
{
	int			mode;

	if (head->file_type == READ_FILE)
	{
        printf("hh\n");
		switch_fds_id(_piped, 6, 1, 1);
		_piped[1] = open(head->args[0], O_RDONLY);
		if (_piped[1] < 0)
			status = switch_fds_id(_piped, 0, 0, 0);
	}
	else if (head->file_type == READ_FROM_APPEND)
	{
		switch_fds_id(_piped, 6, 1, 1);
		status = here_doc_execution(head->args[0], _piped, env);
		signal(SIGINT, handle_ctrl_c);
        //printf("gg");
	}
	else
	{
        printf("ff\n");
		switch_fds_id(_piped, 7, 2, 1);
		mode = O_TRUNC;
		if (head->file_type == WRITE_FILE_APPEND)
			mode = O_APPEND;
		_piped[2] = open(head->args[0], O_WRONLY | O_CREAT | mode, 0666);
	}
	return (status);
}

int check_builtin_cmd(char *cmd)
{
    char *tmp_cmd;
    int status;

    status = 0;
    tmp_cmd = malloc(sizeof_str(cmd, ' ') + 1);
    if(!tmp_cmd)
        return 0;
    s_strcopy(tmp_cmd, cmd,0, sizeof_str(cmd, ' '));
    if(str_cmp(tmp_cmd, "echo","cd")
        || str_cmp(tmp_cmd, "pwd","export")
        || str_cmp(tmp_cmd, "unset","env")
        || str_cmp(tmp_cmd, "exit",NULL))
        status = 1;
    free(tmp_cmd);
    return status;
}

int builtin_cmd_exec_in_child(char **cmd, t_env *env, int *out_fd, int *piped)
{
    int status;

    status = 0;
    (void)piped;
    if(str_cmp(cmd[0], "echo", NULL))
        status = echo_cmd(cmd, out_fd);
	else if(str_cmp(cmd[0], "pwd", "env"))
		status = env_or_pwd_cmd(cmd[0], env, 0, out_fd);
	else if (str_cmp(cmd[0], "export", "unset"))
		cmd = unset_or_export_cmd(cmd,env,out_fd, &status);
	else if(str_cmp(cmd[0], "cd", NULL))
		status = cd_cmd(cmd,env,out_fd);
    else
        write(out_fd[1], "no\n", 3);
   free_string_array(cmd);
    return (status);
}