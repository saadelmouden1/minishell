

int	execute_builtin_with_piping(char **_cmd_, int *_fd, t_env *env, int *_piped)
{
	int				status;
	int				_out_fd[2];

	_out_fd[1] = 1;
	if (_piped[0] > 1)
		pipe(_out_fd);
   // write(_out_fd[1], "tt\n", 3);
	status = builtin_cmd_exec_in_child(_cmd_, env, _out_fd, _piped);
	if (_piped[0] > 1)
	{
		close(_out_fd[1]);
		_fd[0] = _out_fd[0];
	}
	return (status);
}

int builtin_with_simple_piping_exec(char **cmd, int *fd,t_env *env , int *piped)
{
    int status;
    int fd_out[2];

    fd_out[1] = 1;

    if(piped[8] && piped[6])
        fd_out[1] = piped[2];
    if(piped[0] > 1 && (!piped[8] || ! piped[7]))
        pipe(fd_out);
    status = builtin_cmd_exec_in_child(cmd, env, fd_out, piped);
    if(piped[7] && piped[8])
    {
        close(fd_out[1]);
        piped[7] = 0;
    }
    if(!piped[6] && !piped[7])
        piped[8] = 0;
    if(piped[0] > 1 && (!piped[8] || !piped[7]))
    {
        close(fd_out[1]);
        fd[0] = fd_out[0];
    }
    return status;
}

int simple_builtin_execution_managment(char **cmd, int * fd,t_env *env, int *piped)
{
    int status;
    int exec_status;

    status = 0;
    if(str_cmp(cmd[0], "exit", NULL))
    {
        //here
    }
    else if(!piped[8])
        status= execute_builtin_with_piping(cmd, fd, env, piped);
    else
        status = builtin_with_simple_piping_exec(cmd,fd,env, piped);
    
    return status;

}