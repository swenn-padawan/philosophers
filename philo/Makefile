
include pimp_makefile.mk

NAME		:= philo

SRCS_DIR	:= src

OBJS_DIR	:= .objs

CC			:= cc

CFLAGS		:= -Wall -Wextra -Werror -g -fsanitize=thread

IFLAGS		:= -I ./includes

SRCS		:= main.c \
			   parsing/parsing.c \
			   utils/ft_atoi.c \
			   utils/ft_strlen.c \
			   utils/ft_memcpy.c \
			   errors/parsing_errors.c \

SRCS	:= $(addprefix $(SRCS_DIR)/, $(SRCS))

OBJS		:= $(addprefix $(OBJS_DIR)/, $(SRCS:%.c=%.o))

RM			:= rm -rf

DIR_UP		= mkdir -p $(@D)

MAKEFLAGS	+= --no-print-directory

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) -o $@ -lpthread

$(OBJS_DIR)/%.o:%.c
	@$(DIR_UP)
	@$(call run_and_test, $(CC) $(CFLAGS) $(IFLAGS) -o $@ -c $<)

clean:
	@$(RM) $(OBJS_DIR)
	@$(RM) $(OBJS)
	@echo "$(RED)objs removed$(RESET)"

fclean: clean
	@echo "$(RED)$(NAME) removed"
	@$(RM) $(NAME)


re:
	@$(MAKE) fclean
	@$(MAKE) all

################################################################################

RED			= \033[31;49m

YELLOW		= \033[0;33m

GREEN		= \033[0;32m

RESET		= \033[0m

################################################################################

.PHONY: all clean fclean re
