# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    doc.mk                                             :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cdumais <cdumais@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/29 15:17:43 by cdumais           #+#    #+#              #
#    Updated: 2024/11/29 15:34:06 by cdumais          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# **************************************************************************** #
# ---------------------------------- PDF ------------------------------------- #
# **************************************************************************** #
# TODO:maybe: add prompt option for french or english version of the pdf
PDF		:= ft_irc_eng.pdf
GIT_URL	:= https://github.com/SaydRomey/42_ressources
PDF_URL	= $(GIT_URL)/blob/main/pdf/$(PDF)?raw=true

pdf: | $(TMP_DIR) ## Opens the PDF instructions
	@curl -# -L $(PDF_URL) -o $(TMP_DIR)/$(PDF)
	@open $(TMP_DIR)/$(PDF) || echo "Please install a compatible PDF viewer"

.PHONY: pdf
# **************************************************************************** #
# ------------------------------ DOCUMENTATION ------------------------------- #
# **************************************************************************** #
# Weechat
URL_WEECHAT	:= https://weechat.org/
URL_WEEDOC	:= https://weechat.org/doc/

# Guides/Tutorials
URL_SMALL	:= https://medium.com/@afatir.ahmedfatir/small-irc-server-ft-irc-42-network-7cee848de6f9
URL_GUIDE	:= https://reactive.so/post/42-a-comprehensive-guide-to-ft_irc/
URL_CHIRC	:= http://chi.cs.uchicago.edu/chirc/intro.html

# Protocols
URL_RFC1459	:= https://datatracker.ietf.org/doc/html/rfc1459
URL_RFC2810	:= https://datatracker.ietf.org/doc/html/rfc2810
URL_RFC2811	:= https://datatracker.ietf.org/doc/html/rfc2811
URL_RFC2812	:= https://datatracker.ietf.org/doc/html/rfc2812
URL_RFC2813	:= https://datatracker.ietf.org/doc/html/rfc2813
URL_RFC7194	:= https://datatracker.ietf.org/doc/html/rfc7194
# ...

doc: ## Offer a list of documentation URL links
	@clear
	@echo "Select documentation subject:"
	@echo "$(ORANGE)Weechat$(RESET)"
	@echo "  0. Weechat home page"
	@echo "  1. Weechat documentation"
	@echo "$(ORANGE)Guides/Tutorials$(RESET)"
	@echo "  100. ft_irc guide - on medium.com"
	@echo "  101. ft_irc guide - on reactive.so"
	@echo "  102. chirc - similar project with good documentation **"
	@echo "$(ORANGE)IRC Protocols - RFC$(RESET)"
	@echo "  1459. IRC Protocol"
	@echo "  2810. Architecture"
	@echo "  2811. Channel Management"
	@echo "  2812. Client Protocol"
	@echo "  2813. Server Protocol"
	@echo "  7194. Default Port for IRC via TLS/SS"

	@read url_choice; \
	case $$url_choice in \
		0) CHOICE=$(URL_WEECHAT);; \
		1) CHOICE=$(URL_WEEDOC);; \
		100) CHOICE=$(URL_SMALL);; \
		101) CHOICE=$(URL_GUIDE);; \
		102) CHOICE=$(URL_CHIRC);; \
		1459) CHOICE=$(URL_RFC1459);; \
		2810) CHOICE=$(URL_RFC2810);; \
		2811) CHOICE=$(URL_RFC2811);; \
		2812) CHOICE=$(URL_RFC2812);; \
		2813) CHOICE=$(URL_RFC2813);; \
		7194) CHOICE=$(URL_RFC7194);; \
		*) echo "Invalid choice. Exiting." ; exit 1;; \
	esac; \
	open $$CHOICE
	@clear
	@echo "Opening documentation..."

.PHONY: doc
# **************************************************************************** #
# ------------------------------ DOCUMENTATION (CPP) ------------------------- #
# **************************************************************************** #
CPP_REF_URL		:=	https://cplusplus.com/reference/
CPP_TUTO_URL	:=	https://cplusplus.com/doc/oldtutorial/

ref: ## Open C++ reference
	@echo "Opening cplusplus reference's url..."
	@$(OPEN) $(CPP_REF_URL);

tuto: ## Open C++ old tutorial
	@echo "Opening cplusplus oldtutorial's url..."
	@$(OPEN) $(CPP_TUTO_URL);

.PHONY: ref tuto
# **************************************************************************** #
# --------------------------------- GITHUB ----------------------------------- #
# **************************************************************************** #
REPO_LINK	:= https://github.com/SaydRomey/ft_irc

repo: ## Open the GitHub repository
	@echo "Opening $(AUTHOR)'s github repo..."
	@open $(REPO_LINK);

.PHONY: repo
