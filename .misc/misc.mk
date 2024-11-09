doc: ## Offer a list of documentation URL links
	@clear
	@echo "Select documentation subject:"
	@echo "  0. Choosing an IRC client"
	@echo "  1. Weechat home page"
	@echo "  2. Weechat dev documentation"
	@echo "  3. Weechat docker image"
	@echo "  4. Internet Relay Chat Protocol (RFC1459)"
	@echo "  5. Internet Relay Chat: Client Protocol (RFC2812)"
	@echo "  6. ft_irc guide"
	@read url_choice; \
	case $$url_choice in \
		0) CHOICE=$(URL_CLIENTS);; \
		1) CHOICE=$(URL_WEECHAT);; \
		2) CHOICE=$(URL_WEEDEV);; \
		3) CHOICE=$(URL_WEEIMG);; \
		4) CHOICE=$(URL_RFC1459);; \
		5) CHOICE=$(URL_RFC2818);; \
		6) CHOICE=$(URL_GUIDE);; \
		*) echo "Invalid choice. Exiting." ; exit 1;; \
	esac; \
	open $$CHOICE
	@clear
	@echo "Opening documentation..."

.PHONY: doc