NAME1 = ft_nm

CC = gcc
SRC = src/ar.c src/file64.c src/file_info_getters_64.c src/nm.c \
	  src/sort.c src/sort_64.c src/util.c
OBJECTS = src/ar.o src/file64.o src/file_info_getters_64.o src/nm.o \
	  src/sort.o src/sort_64.o src/util.o
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
