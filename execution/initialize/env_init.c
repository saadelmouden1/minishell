

char **duplicate_env_varibles(char **env)
{
    int a;
    int b;
    char **new_env;
    
    a = 0;
    b = 0;
    while(env[a])
        a++;
    new_env = malloc((a + 1) *sizeof(char **));
    if(!new_env)
        return NULL;
    while(b < a)
    {
        new_env[b] = strcopy(env[b]);
        b++;
    }
    new_env[b] = NULL;
    return new_env;
}

int env_structer_shell_initializing(t_env *env, char **original_env, int a)
{

    int b;
    int c;

    env->original_env = duplicate_env_varibles(original_env);
    while(original_env[a])
        a++;
    env->parsed_env = malloc((a +1) * sizeof(char ***));
    if(!env->parsed_env)
        return 0;
    b = -1;
    while(++b < a)
    {
        c = sizeof_str(original_env[b], '=');
        env->parsed_env[b] = malloc(2 * sizeof(char **));
        env->parsed_env[b][0] = malloc(c * sizeof(char *));
        env->parsed_env[b][1] = malloc((sizeof_str(original_env[b], '\0') - c)* sizeof(char *));
        if(!env->parsed_env[b] || !env->parsed_env[b][0] || !env->parsed_env[b][1])
        return 0;
        s_strcopy(env->parsed_env[b][0], original_env[b], 0, c);
       s_strcopy(env->parsed_env[b][1], original_env[b], c+1, sizeof_str(original_env[b],'\0'));
    }
    return (env->parsed_env[b] =0,1);
}  


int shell_initializing_with_env(t_env *env, char **original_env)
{
    int index;
    int status;
    int a;

    
    if(!env)
    return 0;
    status = env_structer_shell_initializing(env, original_env, 0);
    a = find_env_var_index(env, "SHLVL");
    index = 0;
    if(a >= 0)
        index = string_to_int(env->parsed_env[a][1]);
    

    return(status);

}