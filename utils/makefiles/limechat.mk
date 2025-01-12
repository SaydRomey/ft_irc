
# ==============================
###@ LimeChat Integration 🟢 🍋
# ==============================

LIMECHAT_APP		:= "LimeChat"
LIMECHAT_PATH		:= /Applications/LimeChat.app
LIMECHAT_CONFIG_URL	:= https://github.com/psychs/limechat/wiki

limechat: check-limechat limechat-config ## Start the LimeChat application and guide the user to connect to the IRC server
	@$(call INFO,LimeChat,,Opening LimeChat...)
	@open $(LIMECHAT_PATH)

check-limechat: ## Verify if LimeChat is installed on the system
	@if [ ! -d $(LIMECHAT_PATH) ]; then \
		$(call WARNING,LimeChat,Application not found at $(LIMECHAT_PATH).); \
		echo "Please download and install LimeChat from $(LIMECHAT_CONFIG_URL)"; \
		exit 1; \
	else \
		$(call SUCCESS,LimeChat,Application is installed.); \
	fi

limechat-config: check-server ## Display LimeChat configuration instructions
	@$(call INFO,LimeChat,Configuring LimeChat to connect to the server...)
	@echo "\nHere’s how to connect to the server:"
	@echo "1. Open LimeChat."
	@echo "2. Go to 'File -> Add Server'."
	@echo "3. Enter the following details:"
	@echo "   Server Address: $(IRC_SERVER_IP)"
	@echo "   Port: $(IRC_SERVER_PORT)"
	@echo "   Password: $(IRC_SERVER_PSWD)"
	@echo "4. Click 'Connect'.\n"
	@echo "For more details, visit: $(LIMECHAT_CONFIG_URL)"

check-server: ## Verify that the IRC server is running
	@$(call CHECK_CONNECTION,$(IRC_SERVER_IP),$(IRC_SERVER_PORT))
	@$(call SUCCESS,Server,IRC server is reachable at $(IRC_SERVER_IP):$(IRC_SERVER_PORT).)

# include $(MAKE_DIR)/limechat.mk # LimeChat Integration Targets
