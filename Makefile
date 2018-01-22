NAME1 = ft_nm

CC = gcc
SRC = src/nm.c
OBJECTS = src/nm.o
INC = -Iinclude -I./libft/includes
CFLAGS = -g -Wall -Werror -Wextra
LFLAGS = -Llibft
all: $(NAME1)
$(NAME1) :
	@make -C libft
	$(CC) $(CFLAGS) $(INC) $(LFLAGS) -lft -o $(NAME1) $(SRC)

clean:
	@make -C ./libft clean
	$(RM) $(OBJECTS)
	$(RM) -rfv ft_nm.dSYM

fclean: clean
	@make -C ./libft fclean
	$(RM) $(NAME1)

re: fclean all

.PHONY: re fclean clean all
