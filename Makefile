
NAME	:= ircserv
AUTHOR	:= cdumais
TEAM	:= "namoisan, jdemers and $(AUTHOR)"

COMPILE	:= c++
CFLAGS	:= -Wall -Werror -Wextra # -pedantic
STD		:= -std=c++98

# Header files
INC_DIR		:= inc
INCLUDES	:= $(addprefix -I, $(shell find $(INC_DIR) -type d))

# Source code files
SRC_DIR	:= src
SRCS	:= $(shell find $(SRC_DIR) -name "*.cpp")

# Object files
OBJ_DIR		:= obj
OBJS		:= $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
OBJ_SUBDIRS	:= $(sort $(dir $(OBJS)))

# Helper makefiles
MK_DIR		:= ./.makefiles

include  $(MK_DIR)/utils.mk
include  $(MK_DIR)/doc.mk
include  $(MK_DIR)/deco.mk

.DEFAULT_GOAL	:= all

.DEFAULT: ## Handle invalid targets
	$(info make: *** No rule to make target '$(MAKECMDGOALS)'.  Stop.)
	@$(MAKE) help $(NPD)

# Main targets
all: $(NAME) ## Compile the Project and create the executable

$(NAME): $(OBJS)
	@$(COMPILE) $(C_FLAGS) $(STD) $(OBJS) $(INCLUDES) -o $@
	@echo "[$(BOLD)$(PURPLE)$(NAME)$(RESET)]\\t$(GREEN)ready$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	@echo "$(CYAN)Compiling...$(ORANGE)\t$(notdir $<)$(RESET)"
	@$(COMPILE) $(C_FLAGS) $(STD) $(INCLUDES) -c $< -o $@
	@printf "$(UP)$(ERASE_LINE)"

clean: ## Remove folder containing object files
	@if [ -n "$(wildcard $(OBJ_DIR))" ]; then \
		$(REMOVE) $(OBJ_DIR); \
		echo "[$(BOLD)$(PURPLE)$(NAME)$(RESET)] \
		$(GREEN)Object files removed$(RESET)"; \
	else \
		echo "[$(BOLD)$(PURPLE)$(NAME)$(RESET)] \
		$(YELLOW)No object files to remove$(RESET)"; \
	fi

fclean: clean ## 'clean' + Remove executable
	@if [ -n "$(wildcard $(NAME))" ]; then \
		$(REMOVE) $(NAME); \
		echo "[$(BOLD)$(PURPLE)$(NAME)$(RESET)] \
		$(GREEN)Executable removed$(RESET)"; \
	else \
		echo "[$(BOLD)$(PURPLE)$(NAME)$(RESET)] \
		$(YELLOW)No executable to remove$(RESET)"; \
	fi

re: fclean all ## 'fclean' + 'all' (Recompile the project)

.PHONY: all clean fclean re
