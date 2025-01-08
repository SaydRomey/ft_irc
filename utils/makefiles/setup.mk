

# Ensure a Docker image exists
define CHECK_IMAGE
	if ! docker images | grep -q "$(1)"; then \
		$(call INFO,Docker,Pulling image '$(1)'...); \
		docker pull $(1); \
	else \
		$(call SUCCESS,Docker,Image '$(1)' is already available.); \
	fi
endef
# $(call CHECK_IMAGE,$(WEECHAT_IMAGE))

# Check if a Docker container exists (running or stopped)
# Sets a shell variable CONTAINER_EXISTS=true/false
define CHECK_CONTAINER_EXISTS
	if docker ps -a --format '{{.Names}}' | grep -q "^$(1)$$"; then \
		CONTAINER_EXISTS=true; \
	else \
		CONTAINER_EXISTS=false; \
	fi
endef
# $(call CHECK_CONTAINER_EXISTS,$(WEECHAT_CONT))
# if [ "$(CONTAINER_EXISTS)" = "true" ]; then \
	$(call STOP_CONTAINER,$(WEECHAT_CONT))

# Stop the container if it is currently running
define STOP_CONTAINER
	if docker ps --format '{{.Names}}' | grep -q "^$(1)$$"; then \
		$(call INFO,Docker,Stopping container '$(1)'...); \
		docker stop $(1) > /dev/null; \
		$(call SUCCESS,Docker,Container '$(1)' was stopped.); \
	fi
endef
# $(call STOP_CONTAINER,$(WEECHAT_CONT))

# Stop and remove the container if it exists
define REMOVE_CONTAINER
	if docker ps -a --format '{{.Names}}' | grep -q "^$(1)$$"; then \
		$(call INFO,Docker,Removing container '$(1)'...); \
		docker stop $(1) > /dev/null || true; \
		docker rm -f $(1) > /dev/null; \
		$(call SUCCESS,Docker,Container '$(1)' was stopped and removed.); \
	fi
endef
# $(call REMOVE_CONTAINER,$(WEECHAT_CONT))
# **************************************************************************** #
# **************************************************************************** #

# ==============================
##@ 🐳 Docker / Weechat
# ==============================

WEECHAT_IMAGE	:= weechat/weechat
WEECHAT_CONT	:= weechat_instance

weechat: get-weechat ## Start the Weechat docker container
	@$(call CHECK_CONNECTION,$(IRC_SERVER_IP),$(IRC_SERVER_PORT))
	@$(call INFO,Weechat,,Starting container and connecting to the IRC server...)
	@docker run --rm -it \
		--name $(WEECHAT_CONT) \
		--network=host \
		$(WEECHAT_IMAGE) \
		--server-connect=$(IRC_SERVER_IP)/$(IRC_SERVER_PORT)

get-weechat: docker-start ## Pull the Weechat docker image
	@$(call CHECK_IMAGE,$(WEECHAT_IMAGE))
	@$(call CHECK_CONTAINER_EXISTS,$(WEECHAT_CONT))
	@if [ "$$CONTAINER_EXISTS" = "true" ]; then \
		$(call STOP_CONTAINER,$(WEECHAT_CONT))
	fi
	@$(call SUCCESS,Docker,Weechat image is ready.)

docker-start: ## Ensure Docker daemon is running
	@if ! docker info > /dev/null 2>&1; then \
		if [ "$(OS)" = "Darwin" ]; then \
			$(call INFO,Docker,,Starting Docker on macOS...); \
			open --background -a Docker; \
		else \
			$(call WARNING,Docker,Please start Docker manually.); \
		fi; \
		while ! docker info > /dev/null 2>&1; do \
			$(call INFO,Docker,,Waiting for Docker to be ready...); \
			sleep 1; \
			$(call UPCUT); \
		done; \
	fi
	@$(call SUCCESS,Docker,Ready)

weechat-clean: ## Cleanup the Weechat container if it exists
	@$(call CHECK_CONTAINER_EXISTS,$(WEECHAT_CONT))
	@if [ "$$CONTAINER_EXISTS" = "true" ]; then \
		$(call REMOVE_CONTAINER,$(WEECHAT_CONT)); \
	else \
		$(call WARNING,Docker,No container '$(WEECHAT_CONT)' to clean up.); \
	fi
