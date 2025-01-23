
# ==============================
##@ 🌀 Weechat (wip)
# ==============================

# Weechat settings: (wip)
# /server add ircserv localhost/6667 -notls -password=your_password
# /set irc.server.ircserv.capabilities ""


WEECHAT_IMAGE	:= weechat/weechat
WEECHAT_CONT	:= weechat

weechat: get-weechat ## Start the Weechat docker container
	@$(call CHECK_CONNECTION,$(IRC_SERVER_IP),$(IRC_SERVER_PORT))
	@$(call INFO,Weechat,,Starting container and connecting to the IRC server...)
	@docker run --rm -it \
		--name $(WEECHAT_CONT) \
		--network=host \
		$(WEECHAT_IMAGE) \
		weechat \
		--server-connect=$(IRC_SERVER_IP)/$(IRC_SERVER_PORT)

get-weechat: docker-start ## Pull the Weechat docker image
	@$(call PULL_IMAGE,$(WEECHAT_IMAGE))
	@$(call STOP_CONTAINER,$(WEECHAT_CONT))
	@$(call SUCCESS,Docker,Weechat image is ready.)

docker-start: ## Ensure Docker daemon is running
	@$(call START_DOCKER,$(OS))
	@$(call SUCCESS,Docker,Ready)

weechat-clean: ## Cleanup the Weechat container if it exists
	@if [ ! -S /var/run/docker.sock ]; then \
		$(call WARNING,Docker,Docker is not running or the socket is not available.); \
		$(call ERROR,\t,Please start the Docker daemon to clean $(WEECHAT_CONT).); \
		exit 0; \
	fi; \
	$(call CHECK_CONTAINER_EXISTS,$(WEECHAT_CONT)); \
	if [ "$$CONTAINER_EXISTS" = "true" ]; then \
		$(call REMOVE_CONTAINER,$(WEECHAT_CONT)); \
	else \
		$(call WARNING,Docker,No container '$(WEECHAT_CONT)' to clean up.); \
	fi

.PHONY: weechat get-weechat docker-start weechat-clean

# run-wee: all ## Start the IRC server and connect Weechat to it (WIP)
# 	@if [ ! -f "$(NAME)" ]; then \
# 		$(call ERROR,Build Failed:,Executable $(NAME) not found!); \
# 		exit 1; \
# 	fi
# 	@$(call CHECK_COMMAND,docker)
# 	@$(call CHECK_PORT,$(IRC_SERVER_PORT))
# 	@$(call INFO,$(NAME),Starting IRC server on: ,$(IRC_SERVER_PORT):$(IRC_SERVER_PORT)...)
# 	@./$(NAME) $(IRC_SERVER_PORT) $(IRC_SERVER_PSWD) & \
# 	sleep 1; \
# 	$(call WAIT_FOR_CONNECTION,$(IRC_SERVER_IP),$(IRC_SERVER_PORT))
# 	@$(call SUCCESS,$(NAME),IRC server is up and running!)
# 	@$(MAKE) weechat $(NPD)
