
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
##@ 🔈 Sound
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
WAV_DIR		:= ./utils/wav
WAV_WELCOME	:= $(WAV_DIR)/welcome.wav
WAV_PUSHIT	:= $(WAV_DIR)/push.wav

pushit: ## push it to the limit
	@aplay $(WAV_PUSHIT)

welcome: ## what can i say
	@aplay $(WAV_WELCOME)

.PHONY: title pushit welcome
