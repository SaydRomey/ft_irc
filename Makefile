
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

# to log test results:
# $(1) $(TEST_SERVER_BIN) $(TEST_PORT) $(TEST_PASSWORD) > logs/$(2).log 2>&1;

# @if [ -d "$(OBJ_DIR)" ]; then \ # to test with multiple nested obj dirs

# **************************************************************************** #

# Project info
NAME		:= ircserv
AUTHOR		:= cdumais
TEAM		:= "namoisan, jdemers and $(AUTHOR)"
REPO_LINK	:= https://github.com/SaydRomey/ft_irc

# Server info
IRC_SERVER_IP	:= 127.0.0.1
IRC_SERVER_PORT	:= 6667
IRC_SERVER_PSWD	:= 4242

# Utility commands
REMOVE	:= rm -rf
NPD		:= --no-print-directory

# Shell commands
OS		:= $(shell uname)
USER	:= $(shell whoami)
TIME	:= $(shell date "+%H:%M:%S")
CORES	:= $(shell sysctl -n hw.ncpu 2>/dev/null || nproc)

# Build configuration
COMPILE		:= c++
C_FLAGS		:= -Wall -Werror -Wextra -std=c++98 -pedantic #-01
# DEP_FLAGS	:= -MMD -MP
# MAKEFLAGS	+= -j$(CORES) # Parallel build (to test)
DEBUG_FLAGS	:= -DDEBUG # -g

# Temporary flags for ignoring warnings (development use only)
IGNORE_FLAGS	:= -Wno-comment
DEBUG_FLAGS		+= $(IGNORE_FLAGS)

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
PDF_DIR		:= tmp_pdf
TMP_DIRS	:= $(PDF_DIR)

# Text colors and style with ANSI
ESC			:= \033
RESET		:= $(ESC)[0m
BOLD		:= $(ESC)[1m
ITALIC		:= $(ESC)[3m
UNDERLINE	:= $(ESC)[4m
RED			:= $(ESC)[91m
GREEN		:= $(ESC)[32m
YELLOW		:= $(ESC)[93m
ORANGE		:= $(ESC)[38;5;208m
BLUE		:= $(ESC)[94m
PURPLE		:= $(ESC)[95m
CYAN		:= $(ESC)[96m
GRAYTALIC	:= $(ESC)[3;90m
UP			:= $(ESC)[A
ERASE_LINE	:= $(ESC)[2K

# **************************************************************************** #
# **************************************************************************** #
# Standardized output macros
INFO		= echo "[$(BOLD)$(PURPLE)$(1)$(RESET)]\t$(ORANGE)$(2)$(RESET)$(GRAYTALIC)$(3)$(RESET)"
SUCCESS		= echo "[$(BOLD)$(PURPLE)$(1)$(RESET)]\t$(GREEN)$(2)$(RESET)"
WARNING		= echo "[$(BOLD)$(PURPLE)$(1)$(RESET)]\t$(YELLOW)$(2)$(RESET)"
ERROR		= echo "❌ Error: $(1)$(RED)$(2)$(RESET)"
UPCUT		= printf "$(UP)$(ERASE_LINE)"
# **************************************************************************** #
# **************************************************************************** #
# # Utility macros

# Check if a command exists
define CHECK_COMMAND
	if ! command -v $(1) > /dev/null; then \
		$(call ERROR,Command Missing:,The required command '$(1)' is not installed.); \
		exit 1; \
	fi
endef
# $(call CHECK_COMMAND,docker)

# Check network connectivity to a specific IP and Port
define CHECK_CONNECTION
	if ! nc -z $(1) $(2); then \
		$(call ERROR,Connection Error,Unable to reach $(1):$(2). Check if the server is running.); \
		exit 1; \
	fi
endef
# $(call CHECK_CONNECTION,$(IRC_SERVER_IP),$(IRC_SERVER_PORT))

# Wait for a specific IP and port to become available
define WAIT_FOR_CONNECTION
	while ! nc -z $(1) $(2); do \
		$(call INFO,Connection,,Waiting for $(1):$(2) to become available...); \
		sleep 1; \
		$(call UPCUT); \
	done
	@$(call SUCCESS,Connection,$(1):$(2) is now reachable!)
endef
# $(call WAIT_FOR_CONNECTION,$(IRC_SERVER_IP),$(IRC_SERVER_PORT))

# Check if a port is available
define CHECK_PORT
	if command -v lsof > /dev/null; then \
		if lsof -i :$(1) | grep LISTEN > /dev/null 2>&1; then \
			$(call ERROR,Port $(1),is already in use!); \
			exit 1; \
		fi; \
	elif command -v netstat > /dev/null; then \
		if netstat -an | grep ":$(1) .*LISTEN" > /dev/null; then \
			$(call ERROR,Port $(1),is already in use!); \
			exit 1; \
		fi; \
	else \
		$(call WARNING,Port Check,Could not determine if port $(1) is in use. Skipping check.); \
	fi
endef
# $(call CHECK_PORT,$(IRC_SERVER_PORT))

# Ensure a Docker image exists
define CHECK_IMAGE
	if ! docker images | grep -q "$(1)"; then \
		$(call INFO,Docker,Pulling image '$(1)'...); \
		docker pull $(1); \
	else \
		$(call SUCCESS,Docker,Image '$(1)' is already available.); \
	fi
endef
# $(call CHECK_IMAGE,$(WEECHAT_IMAGE))

# Check if a Docker container exists (running or stopped)
# Sets a shell variable CONTAINER_EXISTS=true/false
define CHECK_CONTAINER_EXISTS
	if docker ps -a --format '{{.Names}}' | grep -q "^$(1)$$"; then \
		CONTAINER_EXISTS=true; \
	else \
		CONTAINER_EXISTS=false; \
	fi
endef
# $(call CHECK_CONTAINER_EXISTS,$(WEECHAT_CONT))
# if [ "$(CONTAINER_EXISTS)" = "true" ]; then \
	$(call STOP_CONTAINER,$(WEECHAT_CONT))

# Stop the container if it is currently running
define STOP_CONTAINER
	if docker ps --format '{{.Names}}' | grep -q "^$(1)$$"; then \
		$(call INFO,Docker,Stopping container '$(1)'...); \
		docker stop $(1) > /dev/null; \
		$(call SUCCESS,Docker,Container '$(1)' was stopped.); \
	fi
endef
# $(call STOP_CONTAINER,$(WEECHAT_CONT))

# Stop and remove the container if it exists
define REMOVE_CONTAINER
	if docker ps -a --format '{{.Names}}' | grep -q "^$(1)$$"; then \
		$(call INFO,Docker,Removing container '$(1)'...); \
		docker stop $(1) > /dev/null || true; \
		docker rm -f $(1) > /dev/null; \
		$(call SUCCESS,Docker,Container '$(1)' was stopped and removed.); \
	fi
endef
# $(call REMOVE_CONTAINER,$(WEECHAT_CONT))
# **************************************************************************** #
# **************************************************************************** #

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

# Include dependency files
# -include $(OBJS:.o=.d)

# ==============================
##@ 🧹 Cleanup
# ==============================

clean: ## Remove object files
	@if [ -n "$(wildcard $(OBJ_DIR))" ]; then \
		$(REMOVE) $(OBJ_DIR); \
		$(call SUCCESS,$(NAME),Object files removed); \
	else \
		$(call WARNING,$(NAME),No object files to remove); \
	fi

fclean: clean ## Remove executable
	@if [ -f "$(NAME)" ]; then \
		$(REMOVE) $(NAME); \
		$(call SUCCESS,$(NAME),Executable removed); \
	else \
		$(call WARNING,$(NAME),No executable to remove); \
	fi

ffclean: fclean ## Remove all generated files and folders
	@if [ -n "$(wildcard $(TMP_DIRS))" ]; then \
		$(REMOVE) $(TMP_DIRS); \
		$(call INFO, $(NAME),Temporary files removed: ,$(TMP_DIRS)); \
	else \
		$(call WARNING,$(NAME),No temporary files to remove); \
	fi

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

# ==============================
##@ 🐳 Docker / Weechat
# ==============================

WEECHAT_IMAGE	:= weechat/weechat
WEECHAT_CONT	:= weechat_instance

weechat: get-weechat ## Start the Weechat docker container
	@$(call CHECK_CONNECTION,$(IRC_SERVER_IP),$(IRC_SERVER_PORT))
	@$(call INFO,Weechat,,Starting container and connecting to the IRC server...)
	@docker run --rm -it \
		--name $(WEECHAT_CONT) \
		--network=host \
		$(WEECHAT_IMAGE) \
		--server-connect=$(IRC_SERVER_IP)/$(IRC_SERVER_PORT)

get-weechat: docker-start ## Pull the Weechat docker image
	@$(call CHECK_IMAGE,$(WEECHAT_IMAGE))
	@$(call CHECK_CONTAINER_EXISTS,$(WEECHAT_CONT))
	@if [ "$$CONTAINER_EXISTS" = "true" ]; then \
		$(call STOP_CONTAINER,$(WEECHAT_CONT))
	fi
	@$(call SUCCESS,Docker,Weechat image is ready.)

docker-start: ## Ensure Docker daemon is running
	@if ! docker info > /dev/null 2>&1; then \
		if [ "$(OS)" = "Darwin" ]; then \
			$(call INFO,Docker,,Starting Docker on macOS...); \
			open --background -a Docker; \
		else \
			$(call WARNING,Docker,Please start Docker manually.); \
		fi; \
		while ! docker info > /dev/null 2>&1; do \
			$(call INFO,Docker,,Waiting for Docker to be ready...); \
			sleep 1; \
			$(call UPCUT); \
		done; \
	fi
	@$(call SUCCESS,Docker,Ready)

weechat-clean: ## Cleanup the Weechat container if it exists
	@$(call CHECK_CONTAINER_EXISTS,$(WEECHAT_CONT))
	@if [ "$$CONTAINER_EXISTS" = "true" ]; then \
		$(call REMOVE_CONTAINER,$(WEECHAT_CONT))
	else \
		$(call WARNING,Docker,No container '$(WEECHAT_CONT)' to clean up.); \
	fi

# ==============================
##@ 📚 Documentation
# ==============================

# ft_irc
PDF_EN		:= ft_irc_eng.pdf
PDF_FR		:= ft_irc_eng.pdf # *!! TODO: set fr pdf in ressource repo
URL_GIT		:= https://github.com/SaydRomey/
URL_PDF		:= $(URL_GIT)42_ressources/blob/main/pdf/

# PDF
pdf: | $(PDF_DIR) ## Opens the PDF instructions
	@clear
	@echo "Choose language: (en/fr)"; \
	read lang_choice; \
	case $$lang_choice in \
		en) PDF=$(PDF_EN);; \
		fr) PDF=$(PDF_FR) ;; \
		*) $(call ERROR,Invalid choice, defaulting to English) ; PDF=$(PDF_EN) ;; \
	esac; \
	curl -# -L $(URL_PDF)$$PDF?raw=true -o $(PDF_DIR)/$$PDF; \
	@open $(PDF_DIR)/$$PDF || echo "Please install a compatible PDF viewer"

$(PDF_DIR):
	@mkdir -p $(PDF_DIR)

# C++ documentation
URL_CPP_REF	:=	https://cplusplus.com/reference/

# Weechat
URL_WEECHAT	:= https://weechat.org/
URL_WEEDOC	:= https://weechat.org/doc/

# Guides/Tutorials
URL_SMALL	:= https://medium.com/@afatir.ahmedfatir/small-irc-server-ft-irc-42-network-7cee848de6f9
URL_GUIDE	:= https://reactive.so/post/42-a-comprehensive-guide-to-ft_irc/
URL_CHIRC	:= http://chi.cs.uchicago.edu/chirc/intro.html
URL_BEEJ	:= https://beej.us/guide/bgnet/pdf/bgnet_a4_c_1.pdf

# Protocols
URL_RFC1459	:= https://datatracker.ietf.org/doc/html/rfc1459
URL_RFC2810	:= https://datatracker.ietf.org/doc/html/rfc2810
URL_RFC2811	:= https://datatracker.ietf.org/doc/html/rfc2811
URL_RFC2812	:= https://datatracker.ietf.org/doc/html/rfc2812
URL_RFC2813	:= https://datatracker.ietf.org/doc/html/rfc2813
URL_RFC7194	:= https://datatracker.ietf.org/doc/html/rfc7194
URL_MODERN	:= https://modern.ircdocs.horse/

doc: ## Show documentation links
	@clear
	@echo "Select documentation subject:"
	@echo "\n$(ORANGE)CPP$(RESET)"
	@echo "  0. CPP documentation"
	@echo "\n$(ORANGE)Weechat$(RESET)"
	@echo "  1. Weechat home page"
	@echo "  2. Weechat documentation"
	@echo "\n$(ORANGE)Guides/Tutorials$(RESET)"
	@echo "  100. ft_irc guide - on medium.com"
	@echo "  101. ft_irc guide - on reactive.so"
	@echo "  102. chirc - similar project with good documentation (MUST READ)**"
	@echo "  103. Beej's guide to network programming"
	@echo "\n$(ORANGE)IRC Protocols - RFC$(RESET)"
	@echo "  1459. IRC Protocol"
	@echo "  2810. Architecture"
	@echo "  2811. Channel Management"
	@echo "  2812. Client Protocol"
	@echo "  2813. Server Protocol"
	@echo "  7194. Default Port for IRC via TLS/SS"
	@echo "  999.  Numerics (Details on numeric replies)"

	@read url_choice; \
	case $$url_choice in \
		0) CHOICE=$(URL_CPP_REF);; \
		1) CHOICE=$(URL_WEECHAT);; \
		2) CHOICE=$(URL_WEEDOC);; \
		100) CHOICE=$(URL_SMALL);; \
		101) CHOICE=$(URL_GUIDE);; \
		102) CHOICE=$(URL_CHIRC);; \
		103) CHOICE=$(URL_BEEJ);; \
		1459) CHOICE=$(URL_RFC1459);; \
		2810) CHOICE=$(URL_RFC2810);; \
		2811) CHOICE=$(URL_RFC2811);; \
		2812) CHOICE=$(URL_RFC2812);; \
		2813) CHOICE=$(URL_RFC2813);; \
		7194) CHOICE=$(URL_RFC7194);; \
		999) CHOICE=$(URL_MODERN)/#numerics;; \
		*) $(call ERROR,Invalid choice:,$$CHOICE, Exiting.); exit 1;; \
	esac; \
	open $$CHOICE
	@clear
	@$(call INFO,,Opening documentation...)

# ==============================
##@ 🛠️  Utility
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
	@$(call INFO,,Opening $(AUTHOR)'s github repo...)
	@open $(REPO_LINK);

CHEATSHEET	:= utils/cheat_sheet.txt ## currently not a real thing...
cheatsheet: ## Display IRC commands cheat sheet (TODO)
	@if [ -f $(CHEATSHEET) ]; then \
		cat $(CHEATSHEET); \
	else \
		$(call WARNING,$(NAME),$(CHEATSHEET) not found.); \
	fi

debug: C_FLAGS += $(DEBUG_FLAGS)
debug: all ## Build with debug flags (e.g., disable specific warnings)
	@$(call SUCCESS,$(NAME),Debug build complete)

# Class creation automatisation
class: ## Automate class creation
	@echo "Enter the class name: "; \
	read classname; \
	classname_upper=`echo $$classname | tr a-z A-Z`; \
	\
	# Check for file existence
	if [ -f $(INC_DIR)/$$classname.hpp ] || [ -f $(SRC_DIR)/$$classname.cpp ]; then \
		read -p "Files exist. Overwrite? [y/N]: " confirm; \
		if [ "$$confirm" != "y" ] && [ "$$confirm" != "Y" ]; then \
			echo "Canceling class creation"; \
			exit 1; \
		fi; \
	fi; \
	\
	# Prompt for .hpp directory
	read -p "Use default header directory '$(INC_DIR)'? [y/Y] for yes, or enter a custom directory: " header_dir; \
	if [ "$$header_dir" = "y" ] || [ "$$header_dir" = "Y" ] || [ -z "$$header_dir" ]; then \
		header_dir="$(INC_DIR)"; \
	fi; \
	mkdir -p $$header_dir; \
	\
	# Prompt for .cpp directory
	read -p "Use default source directory '$(SRC_DIR)'? [y/Y] for yes, or enter a custom directory: " source_dir; \
	if [ "$$source_dir" = "y" ] || [ "$$source_dir" = "Y" ] || [ -z "$$source_dir" ]; then \
		source_dir="$(SRC_DIR)"; \
	fi; \
	mkdir -p $$source_dir; \
	\
	# Generate header and source files
	echo "$$CLASS_HEADER" \
	| sed "s/CLASSNAME_UPPER/$$classname_upper/g" \
	| sed "s/CLASSNAME/$$classname/g" > $$header_dir/$$classname.hpp; \
	echo "$$CLASS_CPP" \
	| sed "s/CLASSNAME/$$classname/g" > $$source_dir/$$classname.cpp; \
	\
	echo "$(GREEN)Class '$$classname' created successfully!$(RESET)"; \
	echo "Header file: $(BOLD)$$header_dir/$$classname.hpp$(RESET)"; \
	echo "Source file: $(BOLD)$$source_dir/$$classname.cpp$(RESET)"

# **************************************************************************** #
# **************************************************************************** # # .hpp template
define CLASS_HEADER

#ifndef CLASSNAME_UPPER_HPP
# define CLASSNAME_UPPER_HPP

class CLASSNAME
{
	public:
		CLASSNAME(void);
		CLASSNAME(const CLASSNAME &other);
		CLASSNAME&	operator=(const CLASSNAME &other);
		~CLASSNAME(void);
	
	private:

};

#endif // CLASSNAME_UPPER_HPP
endef

export CLASS_HEADER
# **************************************************************************** #
# **************************************************************************** # # .cpp template
define CLASS_CPP

#include "CLASSNAME.hpp"

CLASSNAME::CLASSNAME(void) {}

CLASSNAME::~CLASSNAME(void) {}

CLASSNAME::CLASSNAME(const CLASSNAME &other)
{
	*this = other;
}

CLASSNAME&	CLASSNAME::operator=(const CLASSNAME &other)
{
	// if (this != &other)
	// {}
	
	return (*this);
}

endef

export CLASS_CPP
# **************************************************************************** #
# **************************************************************************** #

# ==============================
##@ 🎨 Decorations
# ==============================

define TITLE

	**************
	*   ft_irc   *
	**************

endef
export TITLE

title: ## Print ft_irc's logo in ASCII art
	@echo "$(BOLD)$(PURPLE)$(NAME)$(GREEN) created$(RESET)"
	@echo "$(GREEN)$$TITLE$(RESET)"
	@echo "Created by $(ORANGE)$(TEAM)$(RESET)\n"
	@echo "Compiled for $(ITALIC)$(BOLD)$(PURPLE)$(USER)$(RESET) \
		Build time: $(CYAN)$(TIME)$(RESET)\n"

# ==============================
##@ Sound
# ==============================

# sound
WAV_DIR		:= .misc/wav
WAV_WELCOME	:= $(WAV_DIR)/welcome.wav
WAV_PUSHIT	:= $(WAV_DIR)/push.wav

pushit: ## push it to the limit
	@aplay $(WAV_PUSHIT)

welcome: ## what can i say
	@aplay $(WAV_WELCOME)

# Define .PHONY targets
.PHONY: all clean fclean ffclean re \
		run run-wee doc pdf repo \
		weechat docker-start weechat-clean \
		help cheatsheet debug class \
		title pushit welcome 

# ==============================
##@ 🧠 Testing (wip)
# ==============================
# Test Configuration
TEST_SERVER_BIN	:= ./$(NAME)
TEST_SCRIPT_DIR	:= ./utils/scripts
TEST_SCRIPTS	:= $(wildcard $(TEST_SCRIPT_DIR)/*.sh)
TEST_PORT		:= $(IRC_SERVER_PORT)
TEST_PASSWORD	:= $(IRC_SERVER_PSWD)

# Individual Test Scripts
TEST_BASIC		:= $(TEST_SCRIPT_DIR)/test_basic.sh
TEST_OPERATOR	:= $(TEST_SCRIPT_DIR)/test_operator.sh
TEST_FILE_TR	:= $(TEST_SCRIPT_DIR)/test_file_transfer.sh
TEST_BOT		:= $(TEST_SCRIPT_DIR)/test_bot.sh
TEST_STRESS		:= $(TEST_SCRIPT_DIR)/test_stress.sh

# Macro to run a test script
define RUN_TEST
	if [ ! -x "$(1)" ]; then \
		$(call ERROR,Test Script Missing: ,Script '$(1)' is not executable or missing.); \
		exit 1; \
	fi
	@$(call INFO,Testing,Running $(2) test...); \
	$(1) $(TEST_SERVER_BIN) $(TEST_PORT) $(TEST_PASSWORD); \
	$(call SUCCESS,Testing,$(2) test completed successfully.)
endef
# Usage: $(call RUN_TEST,script_path,Test Name)

test: ## Interactive test selection menu
	@echo "Choose a test to run:"
	@echo "1) Basic Functionality"
	@echo "2) Operator Commands"
	@echo "3) File Transfer"
	@echo "4) Bot Interaction"
	@echo "5) Stress Test"
	@echo "6) Run All Tests"
	@echo "7) Clean Test Artifacts"
	@read -p "Enter your choice (1-7): " choice; \
	case $$choice in \
		1) $(MAKE) test_basic $(NPD);; \
		2) $(MAKE) test_operator $(NPD);; \
		3) $(MAKE) test_file_transfer $(NPD);; \
		4) $(MAKE) test_bot $(NPD);; \
		5) $(MAKE) test_stress $(NPD);; \
		6) $(MAKE) tests_all $(NPD);; \
		7) $(MAKE) test_clean $(NPD);; \
		*) $(call ERROR,Invalid Choice:,Please select a number between 1 and 7.);; \
	esac

# Ensure all scripts have execution rights
exec_rights:
	@chmod +x $(TEST_SCRIPTS)
# @$(call SUCCESS,Testing,Scripts in $(TEST_SCRIPTS_DIR) are now executable.)

## Run all tests sequentially
test_all: exec_rights test_basic test_operator test_file_transfer test_bot test_stress
	@$(call SUCCESS,Testing,All)

## Run basic functionality tests
test_basic: exec_rights
	@$(call RUN_TEST,$(TEST_BASIC),Basic Functionality)

## Run operator command tests
test_operator: exec_rights
	@$(call RUN_TEST,$(TEST_OPERATOR),Operator Commands)

## Run file transfer tests
test_file_transfer: exec_rights
	@$(call RUN_TEST,$(TEST_FILE_TR),File Transfer)

## Run bot interaction tests
test_bot: exec_rights
	@$(call RUN_TEST,$(TEST_BOT),Bot Interaction)

## Run stress tests
test_stress: exec_rights
	@$(call RUN_TEST,$(TEST_STRESS),Stress Test)

## Clean test artifacts
test_clean: ## Clean up test artifacts
	@$(call INFO,Testing,Cleaning up test artifacts...)
	@$(REMOVE) testfile.txt received_file.txt split_files/
	@$(call SUCCESS,Testing,Test artifacts removed.)
