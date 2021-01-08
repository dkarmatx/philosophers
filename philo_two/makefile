CC		= clang
STD		= c99
FLAGS	= -Wall -Werror -Wextra -O2
LIBS	= -lpthread

INC_DIR = inc
SRC_DIR	= src
OBJ_DIR = build

NAME	= philo_two

SRCS	=	arguments.c \
			log_q.c \
			logger_write.c \
			logger.c \
			main.c \
			philo_forkpool.c \
			philo_sync.c \
			philo_worker.c \
			philo_pool.c \
			timer.c \
			utils.c \
			sems.c

INCS	= $(patsubst %, -I%, $(INC_DIR))
DEPS	= $(patsubst %, $(OBJ_DIR)/%.d, $(SRCS))
OBJS	= $(patsubst %, $(OBJ_DIR)/%.o, $(SRCS))

.PHONY: all clean re fclean

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(LIBS) $(OBJS) -o $@

$(OBJ_DIR)/%.c.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(FLAGS) -std=$(STD) $(INCS) -o $@ -c -MMD $<

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

-include $(DEPS)
