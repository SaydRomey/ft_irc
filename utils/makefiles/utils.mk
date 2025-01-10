
# Utility commands
REMOVE	:= rm -rf
NPD		:= --no-print-directory

# System Information
OS		:= $(shell uname)
USER	:= $(shell whoami)
TIME	:= $(shell date "+%H:%M:%S")

# Platform-specific adjustments
ifeq ($(OS), Linux)
	C_FLAGS += -Wno-error=implicit-fallthrough -Wimplicit-fallthrough=0
	OPEN		:= xdg-open
	PLAY_SOUND	:= aplay
else ifeq ($(OS), Darwin)
	OPEN		:= open
	PLAY_SOUND	:= afplay
endif

# Default values if OS detection fails
OPEN		?= open
PLAY_SOUND	?= afplay

# Need to add these to platform-specific adjustments for mac or linux, maybe default ?
# OPEN
# PLAY_SOUND (afplay(Darwin) or aplay(Linux))

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

INFO	= echo "[$(BOLD)$(PURPLE)$(1)$(RESET)]\t$(ORANGE)$(2)$(RESET)$(GRAYTALIC)$(3)$(RESET)"
SUCCESS	= echo "[$(BOLD)$(PURPLE)$(1)$(RESET)]\t$(GREEN)$(2)$(RESET)"
WARNING	= echo "[$(BOLD)$(PURPLE)$(1)$(RESET)]\t$(YELLOW)$(2)$(RESET)"
ERROR	= echo "❌ Error: $(1)$(RED)$(2)$(RESET)"
UPCUT	= printf "$(UP)$(ERASE_LINE)"

# ==============================
# Utility Macros
# ==============================

define CLEANUP
	if [ -n "$(wildcard $(3))" ]; then \
		$(REMOVE) $(3); \
		$(call SUCCESS,$(1),$(if $(strip $(4)),$(4),Removed $(2).)); \
	else \
		$(call WARNING,$(1),$(if $(strip $(5)),$(5),No $(2) to remove.)); \
	fi
endef


define CHECK_COMMAND
	if ! command -v $(1) > /dev/null; then \
		$(call ERROR,Command Missing:,The required command '$(1)' is not installed.); \
		exit 1; \
	fi
endef

define CHECK_CONNECTION
	if ! nc -z $(1) $(2); then \
		$(call ERROR,Connection Error,Unable to reach $(1):$(2). Check if the server is running.); \
		exit 1; \
	fi
endef

define WAIT_FOR_CONNECTION
	while ! nc -z $(1) $(2); do \
		$(call INFO,Connection,,Waiting for $(1):$(2) to become available...); \
		sleep 1; \
		$(call UPCUT); \
	done
	@$(call SUCCESS,Connection,$(1):$(2) is now reachable!)
endef

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
