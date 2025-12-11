# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nkuydin <nkuydin@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/26 21:24:13 by nkuydin           #+#    #+#              #
#    Updated: 2025/12/09 22:07:22 by nkuydin          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# patsubst - "pattern substitute", takes each file matching a pattern
# and rewrites it into another pattern 

# $@ == ${NAME}

NAME = pipex

CC = cc
CFLAGS = -Wall -Wextra -Werror

LIBFT_DIR = 42Libft/
OBJ_DIR = objects

HEADERS = -I ${LIBFT_DIR}
SRCS = srcs/pipex.c \
		srcs/get_path.c \
		srcs/utils.c \
		srcs/mandatory.c \
		srcs/multiple_cmd.c \
		srcs/here_doc.c \
		srcs/close.c

OBJS = $(patsubst srcs/%.c,$(OBJ_DIR)/%.o,$(SRCS))

all: libft ${NAME}

${NAME}: ${OBJS}
	@${CC} ${CFLAGS} ${OBJS} ${LIBFT_DIR}/libft.a -o $@
	@echo "${NAME} created successfully."

${OBJ_DIR}/%.o: srcs/%.c | ${OBJ_DIR}
	@mkdir -p ${dir $@}
	@${CC} ${CFLAGS} ${HEADERS} -c $< -o $@
	@echo "Compiled: $<"

${OBJ_DIR}:
	@mkdir -p ${OBJ_DIR}
	@echo "Created objects directory"

libft:
	@echo "Building libft..."
	@make -C ${LIBFT_DIR} > /dev/null
	@echo "Libft compiled properly"

clean:
	@rm -rf ${OBJ_DIR}
	@make -C ${LIBFT_DIR}
	@echo "Object files removed"

fclean: clean
	@rm -rf ${NAME} clean
	@make -C ${LIBFT_DIR} fclean
	@echo "${NAME} removed"

re: fclean all

test: leaks

leaks:
	valgrind --leak-check=full --show-leak-kinds=all ./${NAME} Makefile "cat" "wc -l" out

.PHONY: all clean fclean re test leaks
