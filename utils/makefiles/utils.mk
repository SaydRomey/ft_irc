
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

# Macro: CLEANUP
# Parameters:
# $(1): Name of the cleanup task (for logging clarity)
# $(2): Files/Directories to clean
# $(3): Optional success message (when cleaned)
# $(4): Optional warning message (when nothing to clean)
define CLEANUP
	if [ -n "$(wildcard $(2))" ]; then \
		$(call INFO,Cleanup,$(1) - Removing $(2)); \
		$(REMOVE) $(2); \
		$(if $(strip $(3)), $(call SUCCESS,Cleanup,$(3)), $(call SUCCESS,Cleanup,$(1) - Successfully cleaned)); \
	else \
		$(if $(strip $(4)), $(call WARNING,Cleanup,$(4)), $(call WARNING,Cleanup,$(1) - Nothing to clean)); \
	fi
endef

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
