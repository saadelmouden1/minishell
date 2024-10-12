

int simple_child_in_builtin(char **cmd,int *fd, t_env *env, int *piped)
{
    pid_t pid;
    int pfd[2];
    int out_fd[2];
    int status;
    (pipe(pfd), pid = fork());
    if(!pid)
    {
        if(piped[0] && piped[0] <= piped[5])
        {
            dup2(fd[0], 0);
        }
        if(piped[0 > 1])
            dup2(pfd[1], 1);
        else    
            close(fd[0]);
        close_pipe_ends(pfd[0], pfd[1]);
        dup2(1, out_fd[1]);
        status=builtin_cmd_exec_in_child(cmd, env, out_fd, piped);
        exit(WEXITSTATUS(status));
    }
    close_pipe_ends(pfd[1], fd[0]);
    if(piped[0] > 1)
        fd[0] = pfd[0];
    else
        close(pfd[0]);
    return 1;
}

void builtin_exec_and_exit(char **cmd, t_env *env, int *out_fd, int *piped)
{
    int status;
    status = builtin_cmd_exec_in_child(cmd, env, out_fd, piped);
    exit(WEXITSTATUS(status));
}

int child_redirection(char **cmd, int *fd, t_env *env, int *piped)
{
    pid_t pid;
    int out_fd[2];

    out_fd[1] = 1;
     write(out_fd[1], "ss\n", 3);
    if(piped[8] && piped[7])
    {
       
        out_fd[1] = piped[2];
    }
    if(piped[0] > 1 && (!piped[8] || !piped[7]))
        pipe(out_fd);
    pid = fork();
    if(!pid)
        builtin_exec_and_exit(cmd, env, out_fd, piped);
    if(piped[8] && piped[7])
    {
        close(out_fd[1]);
        piped[7] = 0;
    }
    if(!piped[6] && !piped[7])
        piped[8] = 0;
    if(piped[0] > 1 && (!piped[7] || !piped[8]))
    {
        close(out_fd[1]);
        fd[0] = out_fd[0];
    }
    return 1;
}

int manage_bultin_exec(char **cmd, int *fd, t_env *env, int *piped)
{
    int status;

    status = 0;
    piped[10] += 1;
    printf("%s\n", cmd[0]);
    //piped[0] = 1;
    if(piped[0])
    {
            printf("aa");
        if(!piped[8])
        {
            status = simple_child_in_builtin(cmd, fd, env, piped);
        }   
        // else
        // {
        //     printf("uu\n");
        //     status = child_redirection(cmd, fd, env, piped);
        // }

       free_string_array(cmd);
    }
    else if(piped[4] || piped[3])
    {
         status = child_redirection(cmd, fd, env, piped);
    }
    else
    {
        printf("ee\n");
        status = simple_builtin_execution_managment(cmd, fd, env, piped);
    }
    return status;
}