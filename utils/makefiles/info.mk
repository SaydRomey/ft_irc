

# ==============================
##@ ℹ️  Info
# ==============================

# Detailed Target Descriptions

DESC_all = Builds the entire project by compiling all source files.\nAnd linking them into the final executable.
DESC_clean = Removes all object files generated during the compilation process.
DESC_fclean = Performs 'clean' and also removes the compiled executable.
DESC_re = Runs 'fclean' followed by 'all' to rebuild the entire project from scratch.
DESC_run = Compiles the project and runs the executable with default arguments.\nYou can specify additional arguments using variables (e.g., make run ARG=xyz).
DESC_run_wee = Starts the IRC server and connects Weechat to it automatically.\nUseful for testing interactions between the server and client.
DESC_help = Displays a list of all available Makefile targets with brief descriptions.
DESC_info = Displays detailed information about a specific Makefile target.


# ==============================
## Display Logic Macros
# ==============================

# Macro to display Makefile targets and descriptions
define DISPLAY_TARGETS
awk 'BEGIN { \
		FS = ":.*##"; \
		INCLUDE_HEADERS = ENVIRON["INCLUDE_HEADERS"];} \
	INCLUDE_HEADERS && /^##@/ { \
		printf "\n$(BOLD)%s$(RESET)\n", substr($$0, 5); \
	} \
	/^[a-zA-Z_0-9-]+:.*?##/ { \
		printf "   $(CYAN)%-15s$(RESET) %s\n", $$1, $$2; \
	}' $(MAKEFILE_LIST)
endef

# ==============================
## Info Related Targets
# ==============================

help: ## Display available 'make' targets
	@echo "\nAvailable targets:"
	@INCLUDE_HEADERS=1 $(DISPLAY_TARGETS)

info: ## Display detailed information about a specific target (wip)
	@clear
	@echo "\nAvailable targets:"
	@INCLUDE_HEADERS=0 $(DISPLAY_TARGETS)
	@echo "\nType the target name for more information (or type 'exit' to quit):"
	@read target; \
	case "$$target" in \
		"exit") exit 0;; \
		"all") description="$(DESC_all)";; \
		"clean") description="$(DESC_clean)";; \
		"fclean") description="$(DESC_fclean)";; \
		"re") description="$(DESC_re)";; \
		"run") description="$(DESC_run)";; \
		"run-wee") description="$(DESC_run_wee)";; \
		"help") description="$(DESC_help)";; \
		"info") description="$(DESC_info)";; \
		*) description="No additional information available for '$$target'.";; \
	esac; \
	echo "\nTarget: $(CYAN)$$target$(RESET)"; \
	echo "Description:"; \
	printf "%b\n" "$$description"

# info: ## Display detailed information about a specific target
# 	@clear
# 	@echo "\nAvailable targets:"
# 	@INCLUDE_HEADERS=0 $(DISPLAY_TARGETS)
# 	@echo "\nType the target name for more information (or type 'exit' to quit):"
# 	@read target; \
# 	if [ "$$target" = "exit" ]; then \
# 		exit 0; \
# 	elif grep -qE "^$$target:.*##" $(MAKEFILE_LIST); then \
# 		echo "\nTarget: $(CYAN)$$target$(RESET)"; \
# 		description_var=INFO_$${target//-/_}; \
# 		description=$$(make -s $(NPD) eval-desc TARGET_VAR=$$description_var); \
# 		if [ -n "$$description" ]; then \
# 			echo "Description:"; \
# 			printf "%b\n" "$$description"; \
# 		else \
# 			$(call WARNING,Info,No additional information available for $$target.); \
# 		fi \
# 	else \
# 		$(call ERROR,Info,Target \'$$target\' not found.); \
# 	fi

# Helper target for 'info'
# eval-desc:
# 	@echo $($(TARGET_VAR))

.PHONY: help info


# help: ## Display available 'make' targets
# 	@echo "\nAvailable targets:"
# 	@awk 'BEGIN {FS = ":.*##";} \
# 		/^[a-zA-Z_0-9-]+:.*?##/ { \
# 			printf "   $(CYAN)%-15s$(RESET) %s\n", $$1, $$2 \
# 		} \
# 		/^##@/ { \
# 			printf "\n$(BOLD)%s$(RESET)\n", substr($$0, 5) \
# 		}' $(MAKEFILE_LIST)

# ==============================
# Project Repository
# ==============================

repo: ## Open the GitHub repository
	@$(call INFO,$(NAME),Opening $(AUTHOR)'s github repo...)
	@open $(REPO_LINK);

.PHONY: repo
