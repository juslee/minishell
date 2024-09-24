# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: welee <welee@student.42singapore.sg>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/07/26 20:15:13 by welee             #+#    #+#              #
#    Updated: 2024/09/24 15:28:02 by welee            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = $(BINS_DIR)/minishell
SRCS = $(wildcard $(SRCS_DIR)/*.c)
OBJS = $(SRCS:$(SRCS_DIR)/%.c=$(OBJS_DIR)/%.o)
INCS = $(wildcard $(INCS_DIR)/*.h)

# Directory Variables ---------------------------------------------------------#
SRCS_DIR = srcs
INCS_DIR = includes
OBJS_DIR = objs
BINS_DIR = bin
TEST_DIR = tests
LIBS_DIR = libs
DIST_DIR = dist
DOCS_DIR = docs
# ---------------------------------------------------------------------------- #

# Library Variables -----------------------------------------------------------#
LIBFT_DIR = $(LIBS_DIR)/libft
LIBFT = $(LIBFT_DIR)/bin/libft.a
LIBFT_LIB = -L$(LIBFT_DIR)/bin -lft
LIBFT_INC = $(LIBFT_DIR)/includes

READLINE_DIR = /opt/homebrew/Cellar/readline/8.2.13
READLINE = $(READLINE_DIR)/lib/libreadline.a
READLINE_LIB = -L$(READLINE_DIR)/lib -lreadline
READLINE_INC = $(READLINE_DIR)/include
# ---------------------------------------------------------------------------- #

# Macro Variables -------------------------------------------------------------#
CC = cc
CFLAGS = -Wall -Wextra -Werror -I$(INCS_DIR) -I$(LIBFT_INC) -I$(READLINE_INC)
LIBC = ar rcs
RM = rm -f
MKDIR = mkdir -p
MAKE = make
CD = cd
CP = cp -r
ECHO = echo
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	SED := sed -i
else ifeq ($(UNAME_S),Darwin)
	SED := sed -i ''
else
	$(error Unsupported OS)
endif
WHOAMI = $(shell whoami)
# ---------------------------------------------------------------------------- #

# Tools Variables -------------------------------------------------------------#
NORM = norminette
NORM_FLAGS = -R CheckForbiddenSourceHeader -R CheckDefine

DOXYGEN = doxygen
DOXYGEN_CONFIG = Doxyfile
# ---------------------------------------------------------------------------- #

# Rules Definition -s----------------------------------------------------------#
all: $(NAME)

$(NAME): $(LIBFT) $(OBJS) | $(BINS_DIR)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT_LIB) $(READLINE_LIB)
	@$(ECHO) "\033[32m$(NAME) compiled\033[0m"

$(BINS_DIR):
	$(MKDIR) $@

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.c $(INCS) | $(OBJS_DIR)
	$(MKDIR) $(@D)
	$(CC) $(CFLAGS) -c $< -o $@
	@$(ECHO) "\033[33mCompiled $@\033[0m"

$(OBJS_DIR):
	$(MKDIR) $@

clean:
	$(RM) $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean
	@$(ECHO) "\033[31m$(NAME) object files removed\033[0m"

fclean: clean
	$(RM) $(NAME)
	$(RM) -r $(DIST_DIR)
	$(MAKE) -C $(LIBFT_DIR) fclean
	@$(ECHO) "\033[31m$(NAME) removed\033[0m"

re: fclean all

tests: $(NAME)
	@$(ECHO) "\033[32mTest completed\033[0m"

norm:
	$(NORM) $(NORM_FLAGS) $(SRCS_DIR) $(INCS_DIR)
	@$(ECHO) "\033[32mNorm check completed\033[0m"

doxygen:
	@$(DOXYGEN) $(DOXYGEN_CONFIG)
	@$(ECHO) "\033[32mDoxygen generated\033[0m"

dist: fclean
	$(MKDIR) $(DIST_DIR)
	$(CP) $(SRCS_DIR) $(INCS_DIR) $(LIBS_DIR) $(DIST_DIR)
	$(CP) Makefile $(DIST_DIR)
	$(SED) 's|^NAME = $$(BINS_DIR)/minishell$$|NAME = minishell|' $(DIST_DIR)/Makefile
	$(SED) '/^\$$(NAME):/ s/ | $$(BINS_DIR)//' $(DIST_DIR)/Makefile
	@$(ECHO) "\033[32mDistribution files copied\033[0m"

.PHONY: all bonus clean fclean re norm doxygen dist
# ---------------------------------------------------------------------------- #
