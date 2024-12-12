
SERVER_BIN	:= ./ircserv
SCRIPTS_DIR	:= ./utils/scripts
SCRIPTS		:= $(wildcard $(SCRIPTS_DIR)/*.sh)
PORT		:= 6667
PASSWORD	:= password123

# Targets
.PHONY: all_test test_basic test_operator test_file_transfer test_bot test_stress clean_tests ensure_scripts_executable

## Ensure all scripts in scripts/ have execution rights
ensure_scripts_executable:
	@chmod +x $(SCRIPTS)
	@echo "Scripts in $(SCRIPTS_DIR) are now executable."

all_tests: ensure_scripts_executable basic operator file_transfer bot stress ## Run all tests

test_basic: ensure_scripts_executable $(SERVER_BIN) ## Basic functionality tests
	@echo "Running basic functionality tests..."
	@$(SCRIPTS_DIR)/test_basic.sh $(SERVER_BIN) $(PORT) $(PASSWORD)

test_operator: ensure_scripts_executable $(SERVER_BIN) ## Operator command tests
	@echo "Running operator command tests..."
	@$(SCRIPTS_DIR)/test_operator.sh $(SERVER_BIN) $(PORT) $(PASSWORD)

test_file_transfer: ensure_scripts_executable $(SERVER_BIN) ## File transfer tests
	@echo "Running file transfer tests..."
	@$(SCRIPTS_DIR)/test_file_transfer.sh $(SERVER_BIN) $(PORT) $(PASSWORD)

test_bot: ensure_scripts_executable $(SERVER_BIN) ## Bot interaction tests
	@echo "Running bot interaction tests..."
	@$(SCRIPTS_DIR)/test_bot.sh $(SERVER_BIN) $(PORT) $(PASSWORD)

test_stress: ensure_scripts_executable $(SERVER_BIN) ## Stress tests
	@echo "Running stress tests..."
	@$(SCRIPTS_DIR)/test_stress.sh $(SERVER_BIN) $(PORT) $(PASSWORD)

clean_tests: ## Clean up test artifacts
	@echo "Cleaning up test artifacts..."
	@rm -rf testfile.txt received_file.txt split_files/
