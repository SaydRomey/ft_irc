
# Project info
NAME		:= ircserv
AUTHOR		:= cdumais
TEAM		:= "namoisan, jdemers and $(AUTHOR)"
REPO_LINK	:= https://github.com/SaydRomey/ft_irc

# Build configuration
COMPILE		:= c++
C_FLAGS		:= -Wall -Werror -Wextra -std=c++98 -pedantic

# Source code files
SRC_DIR		:= irc
SRCS		:= $(shell find $(SRC_DIR) -name "*.cpp")

# Object files
OBJ_DIR		:= obj
OBJS		:= $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Header files (including .ipp)
INC_DIR		:= irc
HEADERS		:= $(shell find $(INC_DIR) -name "*.hpp" -o -name "*.ipp")
INCLUDES	:= $(addprefix -I, $(shell find $(INC_DIR) -type d))

# Helper makefiles
MAKE_DIR	:= ./utils/makefiles

include $(MAKE_DIR)/utils.mk	# Output Formatting
include $(MAKE_DIR)/info.mk		# Target Descriptions
include $(MAKE_DIR)/doc.mk		# Documentation Targets
include $(MAKE_DIR)/weechat.mk	# Docker and Weechat Logic
include $(MAKE_DIR)/tests.mk	# Testing Logic
include $(MAKE_DIR)/class.mk	# Class Creation with templates
include $(MAKE_DIR)/misc.mk		# Title and Sounds (not really relevant...)

# ==============================
##@ 🎯 Compilation
# ==============================

.DEFAULT_GOAL	:= all

.DEFAULT:
	$(info make: *** No rule to make target '$(MAKECMDGOALS)'.  Stop.)
	@$(MAKE) help $(NPD)

all: $(NAME) ## Buld the project

$(NAME): $(OBJS)
	@$(COMPILE) $(C_FLAGS) $(OBJS) $(INCLUDES) -o $@
	@$(call SUCCESS,$@,Build complete)

# Object compilation rules
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(HEADERS)
	@mkdir -p $(@D)
	@$(call INFO,$(NAME),Compiling...\t,$(notdir $<))
	@$(COMPILE) $(C_FLAGS) $(INCLUDES) -c $< -o $@
	@$(call UPCUT)

# ==============================
##@ 🧹 Cleanup
# ==============================

clean: ## Remove object files
	@$(call CLEANUP,$(NAME),object files,$(OBJ_DIR))

fclean: clean ## Remove executable
	@$(call CLEANUP,$(NAME),executable,$(NAME))

ffclean: fclean ## Remove all generated files and folders
	@$(MAKE) pdf_clean $(NPD)
	@$(MAKE) test_clean $(NPD)
	@$(MAKE) test_logs_clean $(NPD)
	@$(MAKE) weechat_clean $(NPD)

re: fclean all ## Rebuild everything

.PHONY: all clean fclean ffclean re

# ==============================
##@ 🚀 Execution
# ==============================

# @$(call SUCCESS,$(NAME),Running...!\n)
run: all ## Compile and run the executable with default arguments
	@$(call CHECK_PORT,$(IRC_SERVER_PORT))
	@$(call INFO,$(NAME),,./$(NAME) \"$(IRC_SERVER_PORT)\" \"$(IRC_SERVER_PSWD)\")
	@./$(NAME) $(IRC_SERVER_PORT) $(IRC_SERVER_PSWD)

run-wee: all ## Start the IRC server and connect Weechat to it
	@if [ ! -f "$(NAME)" ]; then \
		$(call ERROR,Build Failed:,Executable $(NAME) not found!); \
		exit 1; \
	fi
	@$(call CHECK_COMMAND,docker)
	@$(call CHECK_PORT,$(IRC_SERVER_PORT))
	@$(call INFO,$(NAME),Starting IRC server on: ,$(IRC_SERVER_PORT):$(IRC_SERVER_PORT)...)
	@./$(NAME) $(IRC_SERVER_PORT) $(IRC_SERVER_PSWD) & \
	sleep 1; \
	@$(call WAIT_FOR_CONNECTION,$(IRC_SERVER_IP),$(IRC_SERVER_PORT))
	@$(call SUCCESS,$(NAME),IRC server is up and running!)
	@$(MAKE) weechat $(NPD)

.PHONY: run run-wee
