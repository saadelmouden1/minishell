

int execute_basic_command(char **_cmd_, int *_fd, char **env, int *_piped)
{
	pid_t				pid;
	int					fd_[2];
    int status;

	pipe(fd_);
	pid = fork();
	signal(SIGINT, child_ctrl_c);
	signal(SIGQUIT, child_ctrl_c);
	if (!pid)
	{
       printf("%s\n",_cmd_[0]);
        //printf("kk");
		if (_piped[0] && _piped[0] <= _piped[5])
			dup2(_fd[0], 0);
		if (_piped[0] > 1)
			dup2(fd_[1], 1);
		else
			close(_fd[0]);
		close_pipe_ends(fd_[0], fd_[1]);
		execve(_cmd_[0], _cmd_, env);
		(ft_putendl_fd(strerror(errno), 2), exit(127));
	}
	close_pipe_ends(fd_[1], _fd[0]);
	if (_piped[0] > 1)
		_fd[0] = fd_[0];
	else
		close(fd_[0]);
   
//   wait(NULL);    // Wait for child process to finish
	return (1);
   
}

int	execute_command_with_redirection(
		char **_cmd_, int *_fd, char **env, int *_piped)
{
	pid_t				pid;
	int					fd_[2];

	pipe(fd_);
	pid = fork();
	signal(SIGINT, child_ctrl_c);
	signal(SIGQUIT, child_ctrl_c);
      //printf("ee");
	if (!pid)
	{
      //printf("a\n");
		child_fds_managment(_piped, _fd, fd_);
		execve(_cmd_[0], _cmd_, env);
		ft_putendl_fd(strerror(errno), 2);
		exit(127);
	}
	parent_fds_managment(_piped, _fd, fd_);
	//free_string_array(_cmd_);
	return (1);
}

int preparation_to_exexcute_command(char **cmd, int *fd, int *piped, t_env *env)
{
   char **cmd_args;
    char **f_args;
    int status;
   // cmd_args = cmd;

f_args = prepare_cmd_arguments(cmd[0], env->original_env, 0);
cmd_args = merge_command_args(f_args, cmd);
    if(check_builtin_cmd(cmd_args[0]))
    {
        printf("%d\n", piped[0]);
        status = manage_bultin_exec(cmd_args, fd, env, piped);
    }
    else{
          piped[10] += 1;
        //  printf("%d\n", piped[10]);
        if(!piped[8])
        {
            status = execute_basic_command(cmd_args, fd, env->original_env,piped);
            printf("%d\n", status);
            free_string_array(cmd_args);
        }else
        {
        //  printf("f3\n");
            status = execute_command_with_redirection( cmd_args, fd,env->original_env, piped);
        }
    }
  
    if(piped[0] > 1)
        piped[0] -= 1;
    return (status);
}

int	wait_for_children(int status, int *_piped)
{
	if (status != 2 && status != 127
		&& status != 126 && _piped[10]
		&& _piped[11])
	{
		while (_piped[10])
		{
			wait(&status);
        //    printf("%d\n",status);
			_piped[10] -= 1;
		}
		signal(SIGINT, handle_ctrl_c);
		signal(SIGQUIT, SIG_IGN);
		if (!g_var_tn)
			return (WEXITSTATUS(status));
		else
			return (g_var_tn);
	}
	return (status);
} 

