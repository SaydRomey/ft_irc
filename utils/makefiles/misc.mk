
# ==============================
##@ 🎨 Decorations
# ==============================

define TITLE

███████╗████████╗     ██╗██████╗  ██████╗
██╔════╝╚══██╔══╝     ██║██╔══██╗██╔════╝
█████╗     ██║        ██║██████╔╝██║     
██╔══╝     ██║        ██║██╔══██╗██║     
██║        ██║███████╗██║██║  ██║╚██████╗
╚═╝        ╚═╝╚══════╝╚═╝╚═╝  ╚═╝ ╚═════╝

endef
export TITLE

title: ## Print ft_irc's logo in ASCII art
	@echo "$(BOLD)$(PURPLE)$(NAME)$(GREEN) created$(RESET)"
	@echo "$(GREEN)$$TITLE$(RESET)"
	@echo "Created by $(BOLD)$(TEAM)$(RESET)"
	@echo "Compiled for $(ITALIC)$(BOLD)$(PURPLE)$(USER)$(RESET) \
		\t$(CYAN)$(TIME)$(RESET)\n"

.PHONY: title

# ==============================
##@ 🔈 Sound
# ==============================

# ifdef OS
# 	ifeq ($(OS), Darwin) # macOS
# 		PLAY_SOUND = afplay
# 	else # Linux
# 		PLAY_SOUND = aplay
# 	endif
# else
# 	$(error "Unsupported OSL Unable to determine sound playback command")
# endif

# Sound Files
WAV_DIR		:= ./utils/wav
WAV_WELCOME	:= $(WAV_DIR)/welcome.wav
WAV_PUSHIT	:= $(WAV_DIR)/push.wav

pushit: ## push it to the limit
	@aplay $(WAV_PUSHIT)

welcome: ## what can i say
	@aplay $(WAV_WELCOME)

.PHONY: pushit welcome
