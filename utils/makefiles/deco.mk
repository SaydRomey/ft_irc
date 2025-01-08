

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
ifdef OS
	ifeq ($(OS), Darwin) # macOS
		PLAY_SOUND = afplay
	else # Linux
		PLAY_SOUND = aplay
	endif
else
	$(error "Unsupported OSL Unable to determine sound playback command")
endif

# Sound Files
WAV_DIR		:= .misc/wav
WAV_WELCOME	:= $(WAV_DIR)/welcome.wav
WAV_PUSHIT	:= $(WAV_DIR)/push.wav

pushit: ## push it to the limit
	@aplay $(WAV_PUSHIT)

welcome: ## what can i say
	@aplay $(WAV_WELCOME)
