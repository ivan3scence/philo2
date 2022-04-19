CFLAGS = -Wall -Wextra -Werror -pthread

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

%.o :	%.c ${HEADERS} Makefile
	${CC} -c ${CFLAGS} $< -o ${<:.c=.o}

${NAME}:	${OBJS} ${HEADERS}
	${CC} ${CFLAGS} ${OBJS} -o $@

clean:
	${RM} ${OBJS}

fclean:		clean
	${RM} ${NAME}

re:	fclean all

