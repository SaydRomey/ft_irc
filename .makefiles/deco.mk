# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    deco.mk                                            :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/29 15:30:03 by cdumais           #+#    #+#              #
#    Updated: 2024/12/06 13:39:08 by cdumais          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

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

# sound
WAV_DIR		:= .misc/wav
WAV_WELCOME	:= $(WAV_DIR)/welcome.wav
WAV_PUSHIT	:= $(WAV_DIR)/push.wav

pushit: ## push it to the limit
	@aplay $(WAV_PUSHIT)

welcome: ## what can i say
	@aplay $(WAV_WELCOME)

.PHONY: pushit welcome
