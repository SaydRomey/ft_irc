
# Utility variables
REMOVE	:= rm -rf
NPD		:= --no-print-directory

# Shell commands
OS		:= $(shell uname)
USER	:= $(shell whoami)
TIME	:= $(shell date "+%H:%M:%S")

# **************************************************************************** #
# ---------------------------------- SETUP ----------------------------------- #
# **************************************************************************** #
IRC_SERVER_PORT	:= 6667
IRC_SERVER_IP	:= 127.0.0.1
WEECHAT_IMAGE	:= weechat/weechat
WEECHAT_CONT	:= weechat_instance

weechat: ## Starts the weechat docker container
	@if ! docker info > /dev/null 2>&1; then \
		echo "Starting Docker..."; \
		open --background -a Docker; \
		while ! docker info > /dev/null 2>&1; do \
			echo "$(GRAYTALIC)Waiting for Docker to be ready...$(RESET)"; \
			sleep 1; \
		done; \
	fi
	@echo "Starting Weechat container..."
	@docker run --rm -it --network=host $(WEECHAT_IMAGE)

.PHONY: weechat
# **************************************************************************** # # (wip - more robust automation of weechat container target...)
# docker-start: ## Ensure Docker daemon is running
# 	@if ! docker info > /dev/null 2>&1; then \
# 		echo "Starting Docker..."; \
# 		open --background -a Docker; \
# 		while ! docker info > /dev/null 2>&1; do \
# 			echo "$(GRAYTALIC)Waiting for Docker to be ready...$(RESET)"; \
# 			sleep 1; \
# 		done; \
# 	fi
# 	@echo "Docker is running."

# weechat: docker-start ## Pull Weechat image and prepare container
# 	@if ! docker images | grep -q "$(WEECHAT_IMAGE)"; then \
# 		echo "Pulling Weechat image..."; \
# 		docker pull $(WEECHAT_IMAGE); \
# 	fi
# 	@if docker ps --format '{{.Names}}' | grep -q "$(WEECHAT_CONT)"; then \
# 		echo "Stopping existing Weechat container..."; \
# 		docker stop $(WEECHAT_CONT); \
# 	fi
# 	@echo "Weechat image is ready."

# clean-weechat: ## Cleanup the Weechat container if it exists
# 	@if docker ps -a --format '{{.Names}}' | grep -q "$(WEECHAT_CONT)"; then \
# 		echo "Removing existing Weechat container..."; \
# 		docker rm -f $(WEECHAT_CONT); \
# 	fi
# 	@echo "Weechat container cleaned up."

# .PHONY: docker-start weechat clean-weechat
# **************************************************************************** # # (wip)
IRC_PORT	:= 6667
IRC_PSWD	:= "42"

run: all ## Compile and run the executable with default arguments
	@echo "$(GRAYTALIC)./$(NAME) \"$(IRC_PORT)\" \"$(IRC_PSWD)\"$(RESET)"
	@./$(NAME) $(IRC_PORT) $(IRC_PSWD)

# run: all weechat ## Starts the IRC server and connects Weechat to it
# 	@echo "Starting IRC server on $(IRC_SERVER_IP):$(IRC_SERVER_PORT)..."
# 	@echo "$(GRAYTALIC)./$(NAME) \"$(IRC_PORT)\" \"$(IRC_PSWD)\"$(RESET)"
# 	@./$(NAME) $(IRC_PORT) $(IRC_PSWD) & \
# 	sleep 2; \

# 	@echo "Starting Weechat and connecting to the IRC server..."
# 	@docker run --rm -it \
# 		--name $(WEECHAT_CONT) \
# 		--network host \
# 		$(WEECHAT_IMAGE) \
# 		--server-connect=$(IRC_SERVER_IP)/$(IRC_SERVER_PORT)

.PHONY: run
# **************************************************************************** #
# ---------------------------------- UTILS ----------------------------------- #
# **************************************************************************** #
help: ## Display available targets
	@echo "\nAvailable targets:"
	@awk 'BEGIN {FS = ":.*##";} \
		/^[a-zA-Z_0-9-]+:.*?##/ { \
			printf "  $(CYAN)%-15s$(RESET) %s\n", $$1, $$2 \
		} \
		/^##@/ { \
			printf "\n$(BOLD)%s$(RESET)\n", substr($$0, 5) \
		}' $(MAKEFILE_LIST)

# TODO: add guide for IRC commands (cheetsheet style)

TMP_DIR	:= tmp

$(TMP_DIR):
	@mkdir -p $(TMP_DIR)

# ffclean: fclean clean-weechat ## 'fclean' + Remove temporary files and folders + Remove weechat container
ffclean: fclean ## 'fclean' + Remove temporary files and folders
	@if [ -n "$(wildcard $(TMP_DIR))" ]; then \
		$(REMOVE) $(TMP_DIR) $(TMP_FILES); \
		echo "[$(BOLD)$(PURPLE)$(NAME)$(RESET)] \
		$(GREEN)Temporary files removed$(RESET)"; \
	else \
		echo "[$(BOLD)$(PURPLE)$(NAME)$(RESET)] \
		$(YELLOW)No temporary files to remove$(RESET)"; \
	fi

.PHONY: help ffclean
# **************************************************************************** #
# --------------------------------- CLASS ------------------------------------ #
# **************************************************************************** #
class: ## Automate class creation
	@echo "Enter the class name: "; \
	read classname; \
	classname_upper=`echo $$classname | tr a-z A-Z`; \
	if [ -f inc/$$classname.hpp ] || [ -f src/$$classname.cpp ]; then \
		read -p "Files exist. Overwrite? [y/N]: " confirm; \
		if [ "$$confirm" != "y" ] && [ "$$confirm" != "Y" ]; then \
			echo "Canceling class creation"; \
			exit 1; \
		fi; \
	fi; \
	mkdir -p $(INC_DIR) $(SRC_DIR); \
	echo "$$CLASS_HEADER" \
	| sed "s/CLASSNAME_UPPER/$$classname_upper/g" \
	| sed "s/CLASSNAME/$$classname/g" > inc/$$classname.hpp; \
	echo "$$CLASS_CPP" \
	| sed "s/CLASSNAME/$$classname/g" > src/$$classname.cpp; \
	echo "$$classname created"

.PHONY: class
# **************************************************************************** #
# ------------------------------- TEMPLATES ---------------------------------- #
# **************************************************************************** #
define CLASS_HEADER
#ifndef CLASSNAME_UPPER_HPP
# define CLASSNAME_UPPER_HPP

# include <iostream>

# define RESET		"\\033[0m"
# define BOLD		"\\033[1m"
# define ITALIC		"\\033[2m"
# define UNDERLINE	"\\033[3m"
# define RED		"\\033[91m"
# define GREEN		"\\033[32m"
# define YELLOW		"\\033[33m"
# define ORANGE		"\\033[38;5;208m"
# define PURPLE		"\\033[95m"
# define CYAN		"\\033[96m"
# define GRAYTALIC	"\\033[3;90m"

class CLASSNAME
{
	public:
		CLASSNAME(void);
		~CLASSNAME(void);
		CLASSNAME(const CLASSNAME &other);
		CLASSNAME&	operator=(const CLASSNAME &other);
	
	private:

};

#endif // CLASSNAME_UPPER_HPP
endef

export CLASS_HEADER
# **************************************************************************** #
# **************************************************************************** #
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
