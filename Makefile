
# Flags to check:

# OPT_FLAGS
# -O1: Basic optimizations, faster compilation.
# -O2: More aggressive optimizations, good balance between performance and compile time.
# -O3: Maximum optimizations, might increase compile time and binary size.

# SECUTIRY_FLAGS
# -fstack-protector: Helps prevent stack buffer overflow attacks.

# EXTRA_WARNINGS
# -Wconversion: Warn about implicit type conversions.
# -Wshadow: Warn when a variable declaration shadows another variable.
# -Wnon-virtual-dtor: Warn if a class with virtual functions has a non-virtual destructor.

# @if [ -d "$(OBJ_DIR)" ]; then \ # to test with multiple nested obj dirs

# **************************************************************************** #

# ==============================
# Core Configuration & Variables
# ==============================

# Project info
NAME		:= ircserv
AUTHOR		:= cdumais
TEAM		:= "namoisan, jdemers and $(AUTHOR)"
REPO_LINK	:= https://github.com/SaydRomey/ft_irc

# Build configuration
COMPILE		:= c++
C_FLAGS		:= -Wall -Werror -Wextra -std=c++98 -pedantic #-01
# MAKEFLAGS	+= -j$(CORES) # Parallel build (to test)

# Platform-specific adjustments
ifeq ($(OS), Linux)
	C_FLAGS += -Wno-error=implicit-fallthrough -Wimplicit-fallthrough=0
endif

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

# Temporary generated files/folders
PDF_DIR			:= tmp_pdf
TEST_LOG_DIR	:= tmp_logs
TMP_DIRS		:= $(PDF_DIR) $(TEST_LOG_DIR)

# Helper makefiles
MK_DIR	:= ./utils/makefiles

include $(MK_DIR)/utils.mk	# ANSI Colors, Output Formatting, Utility Targets and Automation
include $(MK_DIR)/setup.mk	# Docker and Weechat Logic
include $(MK_DIR)/doc.mk	# Documentation Targets
include $(MK_DIR)/tests.mk	# Testing Logic
include $(MK_DIR)/deco.mk	# Title and Sounds (not really relevant...)

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
	@$(COMPILE) $(C_FLAGS) $(DEP_FLAGS) $(INCLUDES) -c $< -o $@
	@$(call UPCUT)

# ==============================
##@ 🧹 Cleanup
# ==============================

# @$(call CLEANUP,$(NAME),Object Files,$(OBJ_DIR),,"No object files to remove.")
clean: ## Remove object files
	@if [ -n "$(wildcard $(OBJ_DIR))" ]; then \
		$(REMOVE) $(OBJ_DIR); \
		$(call SUCCESS,$(NAME),Object files removed); \
	else \
		$(call WARNING,$(NAME),No object files to remove); \
	fi

# @$(call CLEANUP,Executable,$(NAME),$(NAME),"Executable removed.","No executable to remove.")
fclean: clean ## Remove executable
	@if [ -f "$(NAME)" ]; then \
		$(REMOVE) $(NAME); \
		$(call SUCCESS,$(NAME),Executable removed); \
	else \
		$(call WARNING,$(NAME),No executable to remove); \
	fi

# @$(call CLEANUP,Temporary Files,$(NAME),$(TMP_DIRS))
ffclean: fclean ## Remove all generated files and folders
	@if [ -n "$(wildcard $(TMP_DIRS))" ]; then \
		$(REMOVE) $(TMP_DIRS); \
		$(call INFO,$(NAME),Temporary files removed: ,$(TMP_DIRS)); \
	else \
		$(call WARNING,$(NAME),No temporary files to remove); \
	fi
	@$(MAKE) weechat-clean $(NPD)

re: fclean all ## Rebuild everything

# ==============================
##@ 🚀 Execution
# ==============================

run: all ## Compile and run the executable with default arguments
	@$(call CHECK_PORT,$(IRC_SERVER_PORT))
	@$(call INFO,$(NAME),,./$(NAME) \"$(IRC_SERVER_PORT)\" \"$(IRC_SERVER_PSWD)\")
	@./$(NAME) $(IRC_SERVER_PORT) $(IRC_SERVER_PSWD)
	@$(call SUCCESS,$(NAME),Running...!\n)

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
	@$(MAKE) $(NPD) weechat

# Define .PHONY targets
.PHONY: all clean fclean ffclean re \
		run run-wee

