
NAME	:= ircserv
AUTHOR	:= cdumais
TEAM	:= "namoisan, jdemers and $(AUTHOR)"

COMPILE	:= c++
CFLAGS	:= -Wall -Werror -Wextra
STD		:= -std=c++98

# Header files
INC_DIR	:= inc
INCS	:= $(wildcard $(INC_DIR)/*hpp)
INCS	+= $(wildcard $(INC_DIR)/*/*hpp) #
HEADERS	:= -I$(INC_DIR)
# TODO: make headerfiles sub-folder inclusion more dynammic instead of hardcoding...
HEADERS	+= -I$(INC_DIR)/parsing
HEADERS	+= -I$(INC_DIR)/testing

# Template files
TMPLT_DIR	:= $(INC_DIR)/templates
TPPS	:= $(wildcard $(TMPLT_DIR)/*tpp)
IPPS	:= $(wildcard $(TMPLT_DIR)/*ipp)

# Source code files
SRC_DIR	:= src
SRCS	:= $(wildcard $(SRC_DIR)/*.cpp)
SRCS	+= $(wildcard $(SRC_DIR)/*/*.cpp)

# Object files
OBJ_DIR	:= obj
OBJS	:= $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

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

$(NAME): $(OBJS) $(INCS)
	@echo "[$(BOLD)$(PURPLE)$(NAME)$(RESET)]\\t$(GREEN)ready$(RESET)"
	@$(COMPILE) $(C_FLAGS) $(STD) $(OBJS) $(HEADERS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp $(INCS) $(TPPS) $(IPPS) | $(OBJ_DIR)
	@echo "$(CYAN)Compiling...$(ORANGE)\t$(notdir $<)$(RESET)"
	@$(COMPILE) $(C_FLAGS) $(STD) $(HEADERS) -c $< -o $@
	@printf "$(UP)$(ERASE_LINE)"

# TODO: make this more dynamic instead of hardcoding each sub-folders
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(OBJ_DIR)/parsing
	@mkdir -p $(OBJ_DIR)/testing
# @mkdir -p $(OBJ_DIR)/...

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
weechat: ## Starts the weechat docker container
	docker run -it weechat/weechat

# TODO: automate opening docker desktop, running the weechat container
# TODO:maybe: automate connection through server with prompt-based script for ease of use

.PHONY: weechat
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

ARG_PORT	:= 6667
ARG_PSWD	:= 

run: all ## Compile and run the executable with default arguments
	@echo "$(GRAYTALIC)./$(NAME) \"$(ARG_PORT)\" \"$(ARG_PSWD)\"$(RESET)"
	@./$(NAME) $(ARG_PORT) $(ARG_PSWD)

$(TMP_DIR):
	@mkdir -p $(TMP_DIR)

ffclean: fclean ## 'fclean' + Remove temporary files and folders
	@if [ -n "$(wildcard $(TMP_DIR))" ]; then \
		$(REMOVE) $(TMP_DIR) $(TMP_FILES); \
		echo "[$(BOLD)$(PURPLE)$(NAME)$(RESET)] \
		$(GREEN)Temporary files removed$(RESET)"; \
	else \
		echo "[$(BOLD)$(PURPLE)$(NAME)$(RESET)] \
		$(YELLOW)No temporary files to remove$(RESET)"; \
	fi

.PHONY: help run ffclean
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
URL_CLIENTS	:= https://libera.chat/guides/clients
URL_WEECHAT	:= https://weechat.org/
URL_WEEDEV	:= https://weechat.org/files/doc/weechat/stable/weechat_dev.en.html
URL_WEEIMG	:= https://hub.docker.com/r/weechat/weechat
URL_RFC1459	:= https://datatracker.ietf.org/doc/html/rfc1459
URL_RFC2812	:= https://datatracker.ietf.org/doc/html/rfc2812
URL_RFC7194	:= https://datatracker.ietf.org/doc/html/rfc7194
URL_IRCV3	:= https://ircv3.net/irc/

URL_MODERN	:= https://modern.ircdocs.horse/
URL_GUIDE	:= https://reactive.so/post/42-a-comprehensive-guide-to-ft_irc/
# ...

doc: ## Offer a list of documentation URL links
	@clear
	@echo "Select documentation subject:"
	@echo "  0. Choosing an IRC client"
	@echo "  1. Weechat home page"
	@echo "  2. Weechat dev documentation"
	@echo "  3. Weechat docker image"
	@echo "$(ORANGE)Protocols$(RESET)"
	@echo "  4. Internet Relay Chat Protocol (RFC1459)"
	@echo "  5. Internet Relay Chat: Client Protocol (RFC2812)"
	@echo "  6. Default Port for Internet Relay Chat (IRC) via TLS/SSL (RFC7194)"
	@echo "  7. IRCv3 Specification"
	@echo "$(ORANGE)Guides/Tutorials$(RESET)"
	@echo "  100. Modern IRC client protocol"
	@echo "  101. ft_irc guide"
	@read url_choice; \
	case $$url_choice in \
		0) CHOICE=$(URL_CLIENTS);; \
		1) CHOICE=$(URL_WEECHAT);; \
		2) CHOICE=$(URL_WEEDEV);; \
		3) CHOICE=$(URL_WEEIMG);; \
		4) CHOICE=$(URL_RFC1459);; \
		5) CHOICE=$(URL_RFC2818);; \
		6) CHOICE=$(URL_RFC7194);; \
		7) CHOICE=$(URL_IRCV3);; \
		100) CHOICE=$(URL_MODERN);; \
		101) CHOICE=$(URL_GUIDE);; \
		*) echo "Invalid choice. Exiting." ; exit 1;; \
	esac; \
	open $$CHOICE
	@clear
	@echo "Opening documentation..."

.PHONY: doc
# **************************************************************************** #
# --------------------------------- GITHUB ----------------------------------- #
# **************************************************************************** #
REPO_LINK	:= https://github.com/SaydRomey/ft_irc

repo: ## Open the GitHub repository
	@echo "Opening $(AUTHOR)'s github repo..."
	@$(OPEN) $(REPO_LINK);

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