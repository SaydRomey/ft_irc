
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

# ==============================
##@ 🛠️  Utility
# ==============================

# Utility commands
REMOVE	:= rm -rf
NPD		:= --no-print-directory

# System Information
OS		:= $(shell uname)
USER	:= $(shell whoami)
TIME	:= $(shell date "+%H:%M:%S")
CORES	:= $(shell sysctl -n hw.ncpu 2>/dev/null || nproc)

# Platform-specific adjustments
ifeq ($(OS), Linux)
	C_FLAGS += -Wno-error=implicit-fallthrough -Wimplicit-fallthrough=0
endif

# Test flags (to remove)
# MAKEFLAGS	+= -j$(CORES) # Parallel build (to test) : (weird output and troubles with 're' target..)

# Temporary flags for ignoring warnings (development use only)
DEBUG_FLAGS		:= -DDEBUG # -g
IGNORE_FLAGS	:= -Wno-comment
DEBUG_FLAGS		+= $(IGNORE_FLAGS)

# ==============================
# Text colors and style with ANSI
# ==============================

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

# ==============================
# Standardized Output Macros
# ==============================

# Macro: INFO
# Logs an informational message with optional additional details.
# Parameters:
# $(1): Context or section name (e.g., task name).
# $(2): Main informational message.
# $(3): Optional additional details.
# Behavior:
# Formats the message with bold and colored context,
# orange for the main message,
# and gray italics for details.
INFO	= echo "[$(BOLD)$(PURPLE)$(1)$(RESET)]\t$(ORANGE)$(2)$(RESET)$(GRAYTALIC)$(3)$(RESET)"

# Macro: SUCCESS
# Logs a success message.
# Parameters:
# $(1): Context or section name (e.g., task name).
# $(2): Success message.
# Behavior:
# Formats the message with bold and colored context
# and green for the success message.
SUCCESS	= echo "[$(BOLD)$(PURPLE)$(1)$(RESET)]\t$(GREEN)$(2)$(RESET)"

# Macro: WARNING
# Logs a warning message.
# Parameters:
# $(1): Context or section name (e.g., task name).
# $(2): Warning message.
# Behavior:
# Formats the message with bold and colored context
# and yellow for the warning message.
WARNING	= echo "[$(BOLD)$(PURPLE)$(1)$(RESET)]\t$(YELLOW)$(2)$(RESET)"

# Macro: ERROR
# Logs an error message and highlights it in red.
# Parameters:
# $(1): Main error context (e.g., error type or task).
# $(2): Detailed error message.
# Behavior:
# Displays the error with a red icon and message for immediate visibility.
ERROR	= echo "❌ Error: $(1)$(RED)$(2)$(RESET)"

# Macro: UPCUT
# Moves the cursor up one line and clears it,
# useful for refreshing messages in loops.
# Behavior:
# Uses ANSI escape codes to move the cursor up and clear the line.
UPCUT	= printf "$(UP)$(ERASE_LINE)"

# ==============================
# Utility Macros
# ==============================

# Macro: CLEANUP
# Parameters:
# $(1): Caller context (e.g., the project name or task name).
# $(2): Name of the cleanup task (for logging clarity).
# $(3): Files/Directories to clean.
# $(4): Optional custom success message (when cleaned).
# $(5): Optional custom warning message (when nothing to clean).
# Behavior:
# Checks if the specified files/directories exist.
# If they exist, logs an info message, removes the files, and logs a success message.
# If they do not exist, logs a warning message.
# Supports optional custom messages for success and warnings.
define CLEANUP
	if [ -n "$(wildcard $(3))" ]; then \
		$(REMOVE) $(3); \
		$(call SUCCESS,$(1),$(if $(strip $(4)),$(4),Removed $(2).)); \
	else \
		$(call WARNING,$(1),$(if $(strip $(5)),$(5),No $(2) to remove.)); \
	fi
endef
# Example Usage:
# $(call CLEANUP,$(NAME),Object Files,$(OBJ_DIR),,"No object files to remove.")
# $(call CLEANUP,$(NAME),Test Artifacts,testfile.txt received_file.txt,"All test artifacts removed.","No artifacts to clean.")

# **************************************************************************** #

# Macro: CHECK_COMMAND
# Check if a specific command is available on the system
# Parameters:
# $(1): Command name to check.
# Behavior:
# Verifies the command's availability using command -v.
# If the command is not found, displays an error message and exits with status 1.
define CHECK_COMMAND
	if ! command -v $(1) > /dev/null; then \
		$(call ERROR,Command Missing:,The required command '$(1)' is not installed.); \
		exit 1; \
	fi
endef
# Example Usage:
# $(call CHECK_COMMAND,docker)

# **************************************************************************** #

# Macro: CHECK_CONNECTION
# Check network connectivity to a specific IP and Port
# Parameters:
# $(1): IP address to check.
# $(2): Port number to check.
# Behavior:
# Uses nc -z to check connectivity to the given IP and port.
# If unreachable, an error message is displayed, and the script exits with status 1.
define CHECK_CONNECTION
	if ! nc -z $(1) $(2); then \
		$(call ERROR,Connection Error,Unable to reach $(1):$(2). Check if the server is running.); \
		exit 1; \
	fi
endef
# Example Usage:
# $(call CHECK_CONNECTION,$(IRC_SERVER_IP),$(IRC_SERVER_PORT))

# **************************************************************************** #

# Macro: WAIT_FOR_CONNECTION
# Wait for a specific IP and port to become available before proceeding
# Parameters:
# $(1): IP address to wait for.
# $(2): Port number to wait for.
# Behavior:
# Continuously checks the IP and port using nc -z.
# Displays an info message every second while waiting.
# Once reachable, displays a success message.
define WAIT_FOR_CONNECTION
	while ! nc -z $(1) $(2); do \
		$(call INFO,Connection,,Waiting for $(1):$(2) to become available...); \
		sleep 1; \
		$(call UPCUT); \
	done
	@$(call SUCCESS,Connection,$(1):$(2) is now reachable!)
endef
# Example Usage:
# $(call WAIT_FOR_CONNECTION,$(IRC_SERVER_IP),$(IRC_SERVER_PORT))

# **************************************************************************** #

# Macro: CHECK_PORT
# Check if a specific port is already in use
# Parameters:
# $(1): Port number to check.
# Behavior:
# First attempts to use lsof to detect the port's status.
# Falls back to netstat if lsof is unavailable.
# If the port is occupied, an error message is displayed, and the script exits with status 1.
# If neither tool is available, a warning message is displayed, and the check is skipped.
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
# Example Usage:
# $(call CHECK_PORT,$(IRC_SERVER_PORT))

# **************************************************************************** #

# ==============================
# Utility Targets
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

# ==============================
# Class creation automation
# ==============================

class: ## Automate class creation
	@echo "Enter the class name: "; \
	read classname; \
	classname_upper=`echo $$classname | tr a-z A-Z`; \
	\
	# Prompt for .hpp directory \
	read -p "Use default header directory '$(INC_DIR)'? [y/Y] for yes, or enter a custom directory: " header_dir; \
	if [ "$$header_dir" = "y" ] || [ "$$header_dir" = "Y" ] || [ -z "$$header_dir" ]; then \
		header_dir="$(INC_DIR)"; \
	fi; \
	mkdir -p "$$header_dir"; \
	\
	# Prompt for .cpp directory \
	read -p "Use default source directory '$(SRC_DIR)'? [y/Y] for yes, or enter a custom directory: " source_dir; \
	if [ "$$source_dir" = "y" ] || [ "$$source_dir" = "Y" ] || [ -z "$$source_dir" ]; then \
		source_dir="$(SRC_DIR)"; \
	fi; \
	mkdir -p "$$source_dir"; \
	\
	# Check for file existence \
	if [ -f "$$header_dir/$$classname.hpp" ] || [ -f "$$source_dir/$$classname.cpp" ]; then \
		read -p "Files exist in selected directories. Overwrite? [y/N]: " confirm; \
		if [ "$$confirm" != "y" ] && [ "$$confirm" != "Y" ]; then \
			echo "Canceling class creation"; \
			exit 1; \
		fi; \
	fi; \
	\
	# Generate header and source files \
	echo "$$CLASS_HEADER" \
	| sed "s/CLASSNAME_UPPER/$$classname_upper/g" \
	| sed "s/CLASSNAME/$$classname/g" > "$$header_dir/$$classname.hpp"; \
	echo "$$CLASS_CPP" \
	| sed "s/CLASSNAME/$$classname/g" > "$$source_dir/$$classname.cpp"; \
	\
	echo "$(GREEN)Class '$$classname' created successfully!$(RESET)"; \
	echo "Header file: $(BOLD)$$header_dir/$$classname.hpp$(RESET)"; \
	echo "Source file: $(BOLD)$$source_dir/$$classname.cpp$(RESET)"

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
