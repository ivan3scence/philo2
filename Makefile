CFLAGS = -Wall -Wextra -Werror -pthread -fsanitize=address

HDRS = philo.h

HDRS_DIR = includes/

CC = cc

RM = rm -f

NAME = philo

SRC = main.c utils.c validate.c philo.c ph2.c

SRC_DIR = src/

HEADERS = $(addprefix ${HDRS_DIR}, ${HDRS})

SOURCES = $(addprefix ${SRC_DIR}, ${SRC})

OBJS = $(patsubst %.c, %.o, ${SOURCES})

.PHONY:	clean all fclean re

all:	${NAME}

%.o :	%.c ${HEADERS}
	${CC} -c ${CFLAGS} $< -o ${<:.c=.o}

${NAME}:	${SOURCES}
	${CC} ${CFLAGS} ${SOURCES} -o $@

clean:
	${RM} ${OBJS}

fclean:		clean
	${RM} ${NAME}

re:	fclean all

