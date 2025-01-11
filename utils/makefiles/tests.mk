
# ==============================
##@ 🧠 Testing (wip)
# ==============================

# Test Configuration
TEST_SERVER_BIN	:= ./$(NAME)
TEST_PORT		:= $(IRC_SERVER_PORT)
TEST_PASSWORD	:= $(IRC_SERVER_PSWD)

# Test Scripts
TEST_SCRIPT_DIR	:= ./utils/scripts
TEST_BASIC		:= $(TEST_SCRIPT_DIR)/test_basic.sh
TEST_OPERATOR	:= $(TEST_SCRIPT_DIR)/test_operator.sh
TEST_FILE_TR	:= $(TEST_SCRIPT_DIR)/test_file_transfer.sh
TEST_BOT		:= $(TEST_SCRIPT_DIR)/test_bot.sh
TEST_STRESS		:= $(TEST_SCRIPT_DIR)/test_stress.sh

# Test Logs and Artifacts
TEST_LOG_DIR	:= tmp_logs
TEST_LOG_FILE	:= $(TEST_LOG_DIR)/test_$(TIMESTAMP).log
TEST_ARTIFACTS	:= testfile.txt recieved_file.txt split_files/

# ==============================
# Test Related Utilty Macros
# ==============================

# Macro: RUN_TEST
# Run a test script with optional logging
# Parameters:
# $(1): Name of the test (for logging and clarity).
# $(2): Path to the test script.
# $(3): Boolean to enable logging
# Behavior:
# Ensures the test script exists and is executable.
# Runs the test script with parameters for server binary, port, and password.
# If logging is enabled, appends the output to a log file.
# Displays appropriate success messages based on logging status.
define RUN_TEST
	if [ ! -f "$(2)" ]; then \
		$(call ERROR,Test Script Missing: ,Script '$(2)' does not exist.); \
		exit 1; \
	fi; \
	if [ ! -x "$(2)" ]; then \
		chmod +x $(2); \
	fi; \
	$(call INFO,Testing,$(ORANGE)Running $(1) test...); \
	if [ "$(3)" = "true" ]; then \
		$(2) $(TEST_SERVER_BIN) $(TEST_PORT) $(TEST_PASSWORD) >> $(TEST_LOG_FILE) 2>&1; \
		$(call SUCCESS,Testing,$(1) test completed $(RESET)); \
		$(call INFO,Testing,Logged in $(TEST_LOG_FILE)); \
	else \
		$(2) $(TEST_SERVER_BIN) $(TEST_PORT) $(TEST_PASSWORD); \
		$(call SUCCESS,Testing,$(1) test completed successfully.); \
	fi
endef
# Example Usage:
# $(call RUN_TEST,Basic Functionality,$(TEST_BASIC))

# **************************************************************************** #

test: | $(TEST_LOG_DIR) ## Interactive test selection menu
	@clear
	@read -p "Do you want to log test results? (y/n): " log_choice; \
	case $$log_choice in \
		y|Y|"") LOG_ENABLED=true; $(call INFO,Testing,Logging enabled.);; \
		*) LOG_ENABLED=false; $(call INFO,Testing,Logging disabled.);; \
	esac; \
	\
	$(call TITLE,Choose a test to run:); \
	echo "1) Basic Functionality"; \
	echo "2) Operator Commands"; \
	echo "3) File Transfer"; \
	echo "4) Bot Interaction"; \
	echo "5) Stress Test"; \
	echo ""; \
	read -p "Enter your choice (1-5): " choice; \
	case $$choice in \
		1) TEST_TYPE="Basic Functionality"; TEST_SCRIPT="$(TEST_BASIC)";; \
		2) TEST_TYPE="Operator Commands"; TEST_SCRIPT=$(TEST_OPERATOR);; \
		3) TEST_TYPE="File Transfer"; TEST_SCRIPT=$(TEST_FILE_TR);; \
		4) TEST_TYPE="Bot Interaction"; TEST_SCRIPT=$(TEST_BOT);; \
		5) TEST_TYPE="Stress Test"; TEST_SCRIPT=$(TEST_STRESS);; \
		*) $(call ERROR,Invalid Choice:,Please select a number between 1 and 5.); exit 1;; \
	esac; \
	$(call RUN_TEST,$$TEST_TYPE,$$TEST_SCRIPT,$$LOG_ENABLED)

test-clean: ## Clean up test artifacts and logs
	@$(call CLEANUP,Testing,test artifacts,testfile.txt received_file.txt,"All test artifacts removed.","No artifacts to clean.")
	@$(call CLEANUP,Testing,log files,tmp_logs,"Test logs removed.","No logs to remove.")

.PHONY: test test_clean
