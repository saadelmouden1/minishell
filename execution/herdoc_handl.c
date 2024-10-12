



int str_comper(char *s1, char *s2, int max)
{
    int a;
    a = 0;
    while(a < max && *s1 && *s2 && s1[a] == s2[a])
        a++;
    if(sizeof_str(s1, '\0') == a && a == max)
        return 1;
    return 0;
}

void exite_herdoc(int a)
{
    (void) a;
    write(1, "\n", 1);
    exit(0);
}

int  is_there_quotes(char *s)
{
    int a;
    a = 0;

    while(a && s[a])
    {
        if(s[a] == 34 || s[a] == 39)
            return 0;
        a++;
    }
    return 1;

}


void read_and_write(int std_out, char *dlmtr, t_env *env, int is_expanded)
{
    char *buff;
    int f_arr[3];

    dlmtr = remove_quotes_from_str(dlmtr, 0,0,0);
    while(1)
    {
        buff = readline(">> ");
        if(!buff || str_comper(dlmtr, buff, sizeof_str(buff, '\n')))
        {
           //write(std_out, "a\n", 2);
            free(buff);
            break;
        }
       
       // write(std_out, "b\n", 2);
        write(std_out, buff, sizeof(buff, '\n'));
        write(std_out, "\n", 1);
        free(buff);
    }
    free(dlmtr);
}

int here_doc_execution(char *delmtr, int * piped, t_env *env)
{
    int out_fd[2];
    pid_t pid;
    int status;


    pipe(out_fd);
    pid = fork();
    signal(SIGINT, SIG_IGN);
   // printf("%s\n", delmtr);
    if(!pid)
    {
        //printf("ss\n");
        signal(SIGINT, exite_herdoc);
        close(out_fd[0]);
        read_and_write(out_fd[1], delmtr,env,  is_there_quotes(delmtr));
        //  write(out_fd[1], delmtr, sizeof(delmtr));
        exit(1);
    }
    waitpid(pid, &status, 0);
    close(out_fd[1]);
    piped[1] = out_fd[0];
    piped[9] = (WEXITSTATUS(status)) - 1;
    if(piped[9] < 0)
        piped[9] +=2;
    piped[11] = status;
    return (piped[9]);
}