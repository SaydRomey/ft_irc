
# TODO: test using .d

NAME	:= ircserv
AUTHOR	:= cdumais
TEAM	:= "namoisan, jdemers and $(AUTHOR)"

COMPILE	:= c++
C_FLAGS	:= -Wall -Werror -Wextra
# C_FLAGS	+= -pedantic
C_FLAGS	+= -std=c++98
# C_FLAGS	+= -j$(shell nproc)

# tmp flags to ignore some warings during development
WERROR_IGNORE	:= -Wno-comment

C_FLAGS += $(WERROR_IGNORE)

# Conditional flag for Linux to allow implicit fall-through in switch statements
ifeq ($(shell uname), Linux)
	C_FLAGS += -Wno-error=implicit-fallthrough -Wimplicit-fallthrough=0
endif

# Source code files
SRC_DIR		:= irc
SRCS		:= $(shell find $(SRC_DIR) -name "*.cpp")

# Object files
OBJ_DIR		:= obj
OBJS		:= $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
# OBJ_SUBDIRS	:= $(sort $(dir $(OBJS)))

# Header files (including .ipp)
INC_DIR		:= irc
HEADERS		:= $(shell find $(INC_DIR) -name "*.hpp" -o -name "*.ipp")
INCLUDES	:= $(addprefix -I, $(shell find $(INC_DIR) -type d))

# Header files
INC_DIR		:= src
HEADERS		:= $(shell find $(INC_DIR) -name "*.h"")
INCLUDES	:= $(addprefix -I, $(shell find $(INC_DIR) -type d))

# Dependency flags for .d files
# DEP_FLAGS	:= -MMD -MP

# Helper makefiles
MK_DIR  := ./utils/makefiles

include  $(MK_DIR)/utils.mk
include  $(MK_DIR)/doc.mk
include  $(MK_DIR)/deco.mk
include  $(MK_DIR)/tests.mk

.DEFAULT_GOAL	:= all

.DEFAULT: ## Handle invalid targets
	$(info make: *** No rule to make target '$(MAKECMDGOALS)'.  Stop.)
	@$(MAKE) help $(NPD)

# Main targets
all: $(NAME) ## Compile the Project and create the executable

$(NAME): $(OBJS)
	@$(COMPILE) $(C_FLAGS) $(OBJS) $(INCLUDES) -o $@
	@echo "[$(BOLD)$(PURPLE)$(NAME)$(RESET)]\\t$(GREEN)ready$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	@mkdir -p $(@D)
	@echo "$(CYAN)Compiling...$(ORANGE)\t$(notdir $<)$(RESET)"
	@$(COMPILE) $(C_FLAGS) $(DEP_FLAGS) $(INCLUDES) -c $< -o $@
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

# Include dependency files
# -include $(OBJS:.o=.d)

.PHONY: all clean fclean re
