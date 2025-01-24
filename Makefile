NAME = minishell
HEADER = minishell.h
CC = cc
CFLAGS = -Wall -Wextra -Werror
RM = rm -f
LIBFT = ./libft/libft.a
READLINE_INCLUDE = $(shell brew --prefix readline)/include
READLINE_LIB = $(shell brew --prefix readline)/lib

FILES = minishell \
		syntax_checker \
		syntax_check_utils \
		tokenize \
		tokenize_utils \
		parse \
		parser_utils \
		utils \
		string_utils \
		string_utils1 \
		env_init \
		manage_env \
		manage_env1 \
		manage_env2 \
		expand_env \
		expand_env1 \
		signal_fun \
		execution \
		builtin_cmd \
		builtin_cmd1 \
		builtin_cmd2 \
		builtin_cmd3 \
		builtins_exec \
		builtins_exec2 \
		cmand_exec_init \
		cmd_exec_control \
		cmd_exec1 \
		herdoc_handl \
		input_cmd_preccessing \
		path_utils \
		path_utils2 \
		process_pipes_managment \
		shell_utils \

SRCS = $(addsuffix .c, $(FILES))

OBJS = $(addsuffix .o, $(FILES))


all: $(LIBFT) $(NAME)

$(LIBFT):
	cd ./libft && make

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ -L$(READLINE_LIB) -lreadline $(LIBFT) -o $(NAME)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -I$(READLINE_INCLUDE) -c $< -o $@

clean:
	$(RM) $(OBJS)
	cd ./libft && make clean

fclean: clean
	$(RM) $(NAME)
	cd ./libft && make fclean

re: fclean all

.PHONY: clean