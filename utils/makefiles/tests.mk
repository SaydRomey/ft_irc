
# ==============================
##@ 🧠 Testing (wip)
# ==============================

# Test Configuration
TEST_SERVER_BIN	:= ./$(NAME)
TEST_SCRIPT_DIR	:= ./utils/scripts
TEST_SCRIPTS	:= $(wildcard $(TEST_SCRIPT_DIR)/*.sh)
TEST_PORT		:= $(IRC_SERVER_PORT)
TEST_PASSWORD	:= $(IRC_SERVER_PSWD)
TEST_LOG_DIR	:= tmp_logs
TEST_LOG_FILE	:= $(TEST_LOG_DIR)/test_$(shell date +"%Y%m%d_%H%M%S").log
TEST_ARTIFACTS	:= testfile.txt recieved_file.txt split_files/

# Individual Test Scripts
TEST_BASIC		:= $(TEST_SCRIPT_DIR)/test_basic.sh
TEST_OPERATOR	:= $(TEST_SCRIPT_DIR)/test_operator.sh
TEST_FILE_TR	:= $(TEST_SCRIPT_DIR)/test_file_transfer.sh
TEST_BOT		:= $(TEST_SCRIPT_DIR)/test_bot.sh
TEST_STRESS		:= $(TEST_SCRIPT_DIR)/test_stress.sh

# Create log directory if it doesn't exist
$(TEST_LOG_DIR):
	@mkdir -p $(TEST_LOG_DIR)

# ==============================
# Test Related Utilty Macros
# ==============================

# Macro: RUN_TEST
# Run a test script with optional logging
# Parameters:
# $(1): Path to the test script.
# $(2): Name of the test (for logging and clarity).
# Behavior:
# Ensures the test script exists and is executable.
# Runs the test script with parameters for server binary, port, and password.
# If logging is enabled, appends the output to a log file.
# Displays appropriate success messages based on logging status.
define RUN_TEST
	if [ ! -f "$(1)" ]; then \
		$(call ERROR,Test Script Missing: ,Script '$(1)' does not exist.); \
		exit 1; \
	fi
	if [ ! -x "$(1)" ]; then \
		chmod +x $(1); \
	fi; \
	@$(call INFO,Testing,Running $(2) test...); \
	if [ "$(LOG_ENABLED)" = "true" ]; then \
		$(1) $(TEST_SERVER_BIN) $(TEST_PORT) $(TEST_PASSWORD) >> $(TEST_LOG_FILE) 2>&1; \
		$(call SUCCESS,Testing,$(2) test completed (logged in $(TEST_LOG_FILE)).); \
	else \
		$(1) $(TEST_SERVER_BIN) $(TEST_PORT) $(TEST_PASSWORD); \
		$(call SUCCESS,Testing,$(2) test completed successfully.); \
	fi
endef
# Example Usage:
# $(call RUN_TEST,$(TEST_BASIC),Basic Functionality)

# **************************************************************************** #

test: | $(TEST_LOG_DIR) ## Interactive test selection menu
	@echo "Do you want to log test results? (y/n): "; \
	read log_choice; \
	case $$log_choice in \
		y|Y) LOG_ENABLED=true; $(call INFO,Testing,Logging enabled. Log file: $(TEST_LOG_FILE));; \
		*) LOG_ENABLED=false; $(call INFO,Testing,Logging disabled.);; \
	esac; \
	\
	echo "Choose a test to run:"
	@echo "1) Basic Functionality"
	@echo "2) Operator Commands"
	@echo "3) File Transfer"
	@echo "4) Bot Interaction"
	@echo "5) Stress Test"
	@echo "6) Run All Tests"
	@echo "7) Clean Test Artifacts"
	@read -p "Enter your choice (1-7): " choice; \
	case $$choice in \
		1) $(MAKE) test_basic $(NPD);; \
		2) $(MAKE) test_operator $(NPD);; \
		3) $(MAKE) test_file_transfer $(NPD);; \
		4) $(MAKE) test_bot $(NPD);; \
		5) $(MAKE) test_stress $(NPD);; \
		6) $(MAKE) test_all $(NPD);; \
		7) $(MAKE) test_clean $(NPD);; \
		*) $(call ERROR,Invalid Choice:,Please select a number between 1 and 7.);; \
	esac

## Run all tests sequentially
test_all: | $(TEST_LOG_DIR)
	@$(call INFO,Testing,Running all tests sequentially...)
	@$(MAKE) test_basic $(NPD) LOG_ENABLED=$(LOG_ENABLED)
	@$(MAKE) test_operator $(NPD) LOG_ENABLED=$(LOG_ENABLED)
	@$(MAKE) test_file_transfer $(NPD) LOG_ENABLED=$(LOG_ENABLED)
	@$(MAKE) test_bot $(NPD) LOG_ENABLED=$(LOG_ENABLED)
	@$(MAKE) test_stress $(NPD) LOG_ENABLED=$(LOG_ENABLED)
	@$(call SUCCESS,Testing,All tests completed successfully!)

## Run basic functionality tests
test_basic:
	@$(call RUN_TEST,$(TEST_BASIC),Basic Functionality)

## Run operator command tests
test_operator:
	@$(call RUN_TEST,$(TEST_OPERATOR),Operator Commands)

## Run file transfer tests
test_file_transfer:
	@$(call RUN_TEST,$(TEST_FILE_TR),File Transfer)

## Run bot interaction tests
test_bot:
	@$(call RUN_TEST,$(TEST_BOT),Bot Interaction)

## Run stress tests
test_stress:
	@$(call RUN_TEST,$(TEST_STRESS),Stress Test)

test_clean: ## Clean up test artifacts
	@$(call $(NAME),Test Artifacts,testfile.txt received_file.txt,"All test artifacts removed.","No artifacts to clean.")

test_logs_clean: ## Remove all test logs (maybe not needed...)
	@$(call $(NAME),log files,tmp_logs)

.PHONY: test test_all \
		test_basic test_operator test_file_transfer test_bot test_stress \
		test_clean test_logs_clean
