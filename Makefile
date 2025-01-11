
# Project info
NAME		:= ircserv
AUTHOR		:= cdumais
TEAM		:= "namoisan, jdemers and $(AUTHOR)"
REPO_LINK	:= https://github.com/SaydRomey/ft_irc

# Server info
IRC_SERVER_IP	:= 127.0.0.1
IRC_SERVER_PORT	:= 6667
IRC_SERVER_PSWD	:= 4242

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

include $(MAKE_DIR)/utils.mk	# Utility Variables and Macros
include $(MAKE_DIR)/docker.mk	# Docker Macros
include $(MAKE_DIR)/doc.mk		# Documentation Targets
include $(MAKE_DIR)/weechat.mk	# Weechat Targets
include $(MAKE_DIR)/tests.mk	# Testing Logic
include $(MAKE_DIR)/class.mk	# Class Creation with Templates
include $(MAKE_DIR)/misc.mk		# Misc, Title and Sounds (not really relevant...)

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
	@$(call INFO,$(NAME),$(ORANGE)Compiling...\t,$(CYAN)$(notdir $<))
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
	@$(MAKE) pdf-clean $(NPD)
	@$(MAKE) test-clean $(NPD)
	@$(MAKE) weechat-clean $(NPD)

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

# run-lime: all ## Start the IRC server and connect Limechat to it

.PHONY: run run-wee #run-lime

# ==============================
##@ 🛠  Utility
# ==============================

help: ## Display available targets
	@echo "\nAvailable targets:"
	@awk 'BEGIN {FS = ":.*##";} \
		/^[a-zA-Z_0-9-]+:.*?##/ { \
			printf "   $(CYAN)%-15s$(RESET) %s\n", $$1, $$2 \
		} \
		/^##@/ { \
			printf "\n$(BOLD)%s$(RESET)\n", substr($$0, 5) \
		}' $(MAKEFILE_LIST)

repo: ## Open the GitHub repository
	@$(call INFO,$(NAME),Opening $(AUTHOR)'s github repo...)
	@open $(REPO_LINK);

.PHONY: help repo