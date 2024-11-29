NAME	:= ircserv
AUTHOR	:= cdumais
TEAM	:= "namoisan, jdemers and $(AUTHOR)"

COMPILE	:= c++
CFLAGS	:= -Wall -Werror -Wextra
STD		:= -std=c++98

# Header files
INC_DIR	:= inc
INCLUDES := $(addprefix -I, $(shell find $(INC_DIR) -type d))

# Source code files
SRC_DIR	:= src
SRCS	:= $(shell find $(SRC_DIR) -name "*.cpp")

# Object files
OBJ_DIR		:= obj
OBJS		:= $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
OBJ_SUBDIRS	:= $(sort $(dir $(OBJS)))

# Utility variables
REMOVE	:= rm -rf
NPD		:= --no-print-directory

OS		:= $(shell uname)
USER	:= $(shell whoami)
TIME	:= $(shell date "+%H:%M:%S")

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
ARG_PORT	:= 6667
ARG_PSWD	:= "42"

run: all ## Compile and run the executable with default arguments
	@echo "$(GRAYTALIC)./$(NAME) \"$(ARG_PORT)\" \"$(ARG_PSWD)\"$(RESET)"
	@./$(NAME) $(ARG_PORT) $(ARG_PSWD)

# run: all weechat ## Starts the IRC server and connects Weechat to it
# 	@echo "Starting IRC server on $(IRC_SERVER_IP):$(IRC_SERVER_PORT)..."
# 	@echo "$(GRAYTALIC)./$(NAME) \"$(ARG_PORT)\" \"$(ARG_PSWD)\"$(RESET)"
# 	@./$(NAME) $(ARG_PORT) $(ARG_PSWD) & \
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
# ---------------------------------- PDF ------------------------------------- #
# **************************************************************************** #
# TODO:maybe: add prompt option for french or english version of the pdf
PDF		:= ft_irc_eng.pdf
GIT_URL	:= https://github.com/SaydRomey/42_ressources
PDF_URL	= $(GIT_URL)/blob/main/pdf/$(PDF)?raw=true

TMP_DIR	:= tmp

$(TMP_DIR):
	@mkdir -p $(TMP_DIR)

pdf: | $(TMP_DIR) ## Opens the PDF instructions
	@curl -# -L $(PDF_URL) -o $(TMP_DIR)/$(PDF)
	@open $(TMP_DIR)/$(PDF) || echo "Please install a compatible PDF viewer"

.PHONY: pdf
# **************************************************************************** #
# ------------------------------ DOCUMENTATION ------------------------------- #
# **************************************************************************** #
# Weechat
URL_WEECHAT	:= https://weechat.org/
URL_WEEDOC	:= https://weechat.org/doc/
# Guides/Tutorials
URL_SMALL	:= https://medium.com/@afatir.ahmedfatir/small-irc-server-ft-irc-42-network-7cee848de6f9
URL_GUIDE	:= https://reactive.so/post/42-a-comprehensive-guide-to-ft_irc/
URL_CHIRC	:= http://chi.cs.uchicago.edu/chirc/intro.html
# Protocols
URL_RFC1459	:= https://datatracker.ietf.org/doc/html/rfc1459
URL_RFC2810	:= https://datatracker.ietf.org/doc/html/rfc2810
URL_RFC2811	:= https://datatracker.ietf.org/doc/html/rfc2811
URL_RFC2812	:= https://datatracker.ietf.org/doc/html/rfc2812
URL_RFC2813	:= https://datatracker.ietf.org/doc/html/rfc2813
URL_RFC7194	:= https://datatracker.ietf.org/doc/html/rfc7194
# ...

doc: ## Offer a list of documentation URL links
	@clear
	@echo "Select documentation subject:"
	@echo "$(ORANGE)Weechat$(RESET)"
	@echo "  0. Weechat home page"
	@echo "  1. Weechat documentation"
	@echo "$(ORANGE)Guides/Tutorials$(RESET)"
	@echo "  100. ft_irc guide - on medium.com"
	@echo "  101. ft_irc guide - on reactive.so"
	@echo "  102. chirc - similar project with good documentation **"
	@echo "$(ORANGE)IRC Protocols - RFC$(RESET)"
	@echo "  1459. IRC Protocol"
	@echo "  2810. Architecture"
	@echo "  2811. Channel Management"
	@echo "  2812. Client Protocol"
	@echo "  2813. Server Protocol"
	@echo "  7194. Default Port for IRC via TLS/SS"

	@read url_choice; \
	case $$url_choice in \
		0) CHOICE=$(URL_WEECHAT);; \
		1) CHOICE=$(URL_WEEDOC);; \
		100) CHOICE=$(URL_SMALL);; \
		101) CHOICE=$(URL_GUIDE);; \
		102) CHOICE=$(URL_CHIRC);; \
		1459) CHOICE=$(URL_RFC1459);; \
		2810) CHOICE=$(URL_RFC2810);; \
		2811) CHOICE=$(URL_RFC2811);; \
		2812) CHOICE=$(URL_RFC2812);; \
		2813) CHOICE=$(URL_RFC2813);; \
		7194) CHOICE=$(URL_RFC7194);; \
		*) echo "Invalid choice. Exiting." ; exit 1;; \
	esac; \
	open $$CHOICE
	@clear
	@echo "Opening documentation..."

.PHONY: doc
# **************************************************************************** #
CPP_REF_URL		:=	https://cplusplus.com/reference/
CPP_TUTO_URL	:=	https://cplusplus.com/doc/oldtutorial/

ref: ## Open C++ reference
	@echo "Opening cplusplus reference's url..."
	@$(OPEN) $(CPP_REF_URL);

tuto: ## Open C++ old tutorial
	@echo "Opening cplusplus oldtutorial's url..."
	@$(OPEN) $(CPP_TUTO_URL);

.PHONY: ref tuto
# **************************************************************************** #
# --------------------------------- GITHUB ----------------------------------- #
# **************************************************************************** #
REPO_LINK	:= https://github.com/SaydRomey/ft_irc

repo: ## Open the GitHub repository
	@echo "Opening $(AUTHOR)'s github repo..."
	@open $(REPO_LINK);

.PHONY: repo
# **************************************************************************** #
# ------------------------------- DECORATIONS -------------------------------- #
# **************************************************************************** #
define TITLE

	***************
	* PLACEHOLDER *
	***************

endef
export TITLE

title: ## Print ft_irc's logo in ASCII art
	@echo "$(BOLD)$(PURPLE)$(NAME)$(GREEN) created$(RESET)"
	@echo "$(GREEN)$$TITLE$(RESET)"
	@echo "$(ORANGE)$(TEAM)$(RESET)\n"
	@echo "Compiled for $(ITALIC)$(BOLD)$(PURPLE)$(USER)$(RESET) \
		$(CYAN)$(TIME)$(RESET)\n"

.PHONY: title
# **************************************************************************** #
# ----------------------------------- ANSI ----------------------------------- #
# **************************************************************************** #
ESC			:= \033

# Text
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

# Cursor movement
UP			:= $(ESC)[A

# Erasing
ERASE_LINE	:= $(ESC)[2K
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
