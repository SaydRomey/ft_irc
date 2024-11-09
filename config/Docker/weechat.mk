# Variables
PREFIX			:= weechat-
IMAGE_NAME		:= weechat
CONTAINER_NAME	:= ft_irc
NETWORK_NAME	:= ft_irc_network
IRC_PORT		:= 6667

# Targets

$(PREFIX)all: $(PREFIX)build $(PREFIX)run

$(PREFIX)build: ## Pulls the weechat image to ensure it's available
	@echo "Building Docker container with Weechat..."
	docker pull $(IMAGE_NAME)

# Stop any existing container with same name
# Create a Docker network if it does not already exist (to mimic an isolated IRC environment)
# Run the Weechat container, binding the IRC port
$(PREFIX)run: $(PREFIX)stop ## 'stop' + Create Docker network and run the Weechat container
	@echo "Creating network (if not exists)..."
	docker network inspect $(NETWORK_NAME) >/dev/null 2>&1 || \
		docker network create $(NETWORK_NAME)
	@echo "Running Weechat container..."
	docker run -d --name $(CONTAINER_NAME) --network $(NETWORK_NAME) \
		-p $(IRC_PORT):$(IRC_PORT) $(IMAGE_NAME)

$(PREFIX)stop: ## Stop and remove any existing container with the same name to avoid conflicts
	@echo "Stopping and removing any existing Weechat container..."
	docker rm -f $(CONTAINER_NAME) || true

$(PREFIX)clean: $(PREFIX)stop ## Stop and remove the container and network
	@echo "Removing network (if exists)..."
	docker network rm $(NETWORK_NAME) || true

.PHONY: $(PREFIX)all $(PREFIX)build $(PREFIX)run $(PREFIX)stop $(PREFIX)clean
