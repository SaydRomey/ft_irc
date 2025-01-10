
# ==============================
##@ 🐳 Docker / Weechat (wip)
# ==============================

# Server info
IRC_SERVER_IP	:= 127.0.0.1
IRC_SERVER_PORT	:= 6667
IRC_SERVER_PSWD	:= 4242

WEECHAT_IMAGE	:= weechat/weechat
WEECHAT_CONT	:= weechat_instance

# ==============================
# Docker Related Utility Macros
# ==============================

# Macro: CHECK_IMAGE
# Ensure a Docker image exists locally, if not, pull the image from the Docker registry
# Parameters:
# $(1): Docker image name.
# Behavior:
# Checks the local Docker image list using docker images.
# If not found, pulls the image using docker pull.
# Displays success messages based on the image's availability.
define CHECK_IMAGE
	if ! docker images | grep -q "$(1)"; then \
		$(call INFO,Docker,Pulling image '$(1)'...); \
		docker pull $(1); \
	else \
		$(call SUCCESS,Docker,Image '$(1)' is already available.); \
	fi
endef
# Example Usage:
# $(call CHECK_IMAGE,$(WEECHAT_IMAGE))

# **************************************************************************** #

# Macro: CHECK_CONTAINER_EXISTS
# Check if a Docker container exists (running or stopped)
# Parameters:
# $(1): Docker container name.
# Behavior:
# Uses docker ps -a to look for the container by name.
# Sets the shell variable CONTAINER_EXISTS to true if found, otherwise false.
define CHECK_CONTAINER_EXISTS
	if docker ps -a --format '{{.Names}}' | grep -q "^$(1)$$"; then \
		CONTAINER_EXISTS=true; \
	else \
		CONTAINER_EXISTS=false; \
	fi
endef
# Example Usage:
# $(call CHECK_CONTAINER_EXISTS,$(WEECHAT_CONT))
# if [ "$(CONTAINER_EXISTS)" = "true" ]; then \
	$(call STOP_CONTAINER,$(WEECHAT_CONT))

# **************************************************************************** #

# Macro: STOP_CONTAINER
# Stop the container if it is currently running
# Parameters:
# $(1): Docker container name.
# Behavior:
# Checks if the container is currently running using docker ps.
# If running, stops the container and displays a success message.
define STOP_CONTAINER
	if docker ps --format '{{.Names}}' | grep -q "^$(1)$$"; then \
		$(call INFO,Docker,Stopping container '$(1)'...); \
		docker stop $(1) > /dev/null; \
		$(call SUCCESS,Docker,Container '$(1)' was stopped.); \
	fi
endef
# Example Usage:
# $(call STOP_CONTAINER,$(WEECHAT_CONT))

# **************************************************************************** #

# Macro: REMOVE_CONTAINER
# Stop and remove the container if it exists
# Parameters:
# $(1): Docker container name.
# Behavior:
# Stops the container if running.
# Removes the container using docker rm -f.
# Displays a success message upon completion.
define REMOVE_CONTAINER
	if docker ps -a --format '{{.Names}}' | grep -q "^$(1)$$"; then \
		$(call INFO,Docker,Removing container '$(1)'...); \
		docker stop $(1) > /dev/null || true; \
		docker rm -f $(1) > /dev/null; \
		$(call SUCCESS,Docker,Container '$(1)' was stopped and removed.); \
	fi
endef
# Example Usage:
# $(call REMOVE_CONTAINER,$(WEECHAT_CONT))

# **************************************************************************** #

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
	@if [ ! -S /var/run/docker.sock ]; then \
		$(call WARNING,Docker,Docker is not running or the socket is not available.); \
		$(call WARNING,Docker,Please start the Docker daemon to clean $(WEECHAT_CONT).); \
		exit 0; \
	fi; \
	$(call CHECK_CONTAINER_EXISTS,$(WEECHAT_CONT)); \
	if [ "$$CONTAINER_EXISTS" = "true" ]; then \
		$(call REMOVE_CONTAINER,$(WEECHAT_CONT)); \
	else \
		$(call WARNING,Docker,No container '$(WEECHAT_CONT)' to clean up.); \
	fi

.PHONY: weechat get-weechat docker-start weechat-clean
