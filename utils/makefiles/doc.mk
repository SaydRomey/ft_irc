
# ==============================
##@ 📚 Documentation
# ==============================

# C++ documentation
URL_CPP_REF	:=	https://cplusplus.com/reference/

# Weechat
URL_WEECHAT	:= https://weechat.org/
URL_WEEDOC	:= https://weechat.org/doc/

# Guides/Tutorials
URL_SMALL	:= https://medium.com/@afatir.ahmedfatir/small-irc-server-ft-irc-42-network-7cee848de6f9
URL_GUIDE	:= https://reactive.so/post/42-a-comprehensive-guide-to-ft_irc/
URL_CHIRC	:= http://chi.cs.uchicago.edu/chirc/intro.html
URL_BEEJ	:= https://beej.us/guide/bgnet/pdf/bgnet_a4_c_1.pdf

# Protocols
URL_RFC1459	:= https://datatracker.ietf.org/doc/html/rfc1459
URL_RFC2810	:= https://datatracker.ietf.org/doc/html/rfc2810
URL_RFC2811	:= https://datatracker.ietf.org/doc/html/rfc2811
URL_RFC2812	:= https://datatracker.ietf.org/doc/html/rfc2812
URL_RFC2813	:= https://datatracker.ietf.org/doc/html/rfc2813
URL_RFC7194	:= https://datatracker.ietf.org/doc/html/rfc7194
URL_MODERN	:= https://modern.ircdocs.horse/

doc: ## Show documentation links
	@clear
	@echo "Select documentation subject:"
	@echo "\n$(ORANGE)CPP$(RESET)"
	@echo "  0. CPP documentation"
	@echo "\n$(ORANGE)Weechat$(RESET)"
	@echo "  1. Weechat home page"
	@echo "  2. Weechat documentation"
	@echo "\n$(ORANGE)Guides/Tutorials$(RESET)"
	@echo "  100. ft_irc guide - on medium.com"
	@echo "  101. ft_irc guide - on reactive.so"
	@echo "  102. chirc - similar project with good documentation"
	@echo "  103. Beej's guide to network programming"
	@echo "\n$(ORANGE)IRC Protocols - RFC$(RESET)"
	@echo "  1459. IRC Protocol"
	@echo "  2810. Architecture"
	@echo "  2811. Channel Management"
	@echo "  2812. Client Protocol"
	@echo "  2813. Server Protocol"
	@echo "  7194. Default Port for IRC via TLS/SS"
	@echo "  999.  Numerics (Details on numeric replies)"

	@read url_choice; \
	case $$url_choice in \
		0) CHOICE=$(URL_CPP_REF);; \
		1) CHOICE=$(URL_WEECHAT);; \
		2) CHOICE=$(URL_WEEDOC);; \
		100) CHOICE=$(URL_SMALL);; \
		101) CHOICE=$(URL_GUIDE);; \
		102) CHOICE=$(URL_CHIRC);; \
		103) CHOICE=$(URL_BEEJ);; \
		1459) CHOICE=$(URL_RFC1459);; \
		2810) CHOICE=$(URL_RFC2810);; \
		2811) CHOICE=$(URL_RFC2811);; \
		2812) CHOICE=$(URL_RFC2812);; \
		2813) CHOICE=$(URL_RFC2813);; \
		7194) CHOICE=$(URL_RFC7194);; \
		999) CHOICE=$(URL_MODERN)/#numerics;; \
		*) $(call ERROR,Invalid choice:,$$CHOICE, Exiting.); exit 1;; \
	esac; \
	open $$CHOICE
	@clear
	@$(call INFO,,Opening documentation...)

.PHONY: doc

# ==============================
# PDF
# ==============================

URL_GIT		:= https://github.com/SaydRomey/
URL_PDF		:= $(URL_GIT)42_ressources/blob/main/pdf/
PDF_DIR		:= tmp_pdf
PDF_NAME	:= ft_irc
PDF			:= $(PDF_DIR)/$(PDF_NAME).pdf

pdf: | $(PDF_DIR) ## Opens the PDF instructions
	@clear
	@echo "Choose language: (en/fr)"; \
	read lang_choice; \
	case $$lang_choice in \
		en) PDF=$(PDF_NAME)_en.pdf;; \
		fr) PDF=$(PDF_NAME)_fr.pdf;; \
		*) $(call ERROR,Invalid choice, defaulting to English); PDF=$(PDF_NAME)_en.pdf;; \
	esac; \
	curl -# -L $(URL_PDF)$$PDF?raw=true -o $(PDF); \
	echo "Opening $(PDF)..."
	@$(OPEN) $(PDF) || echo "Please install a compatible PDF viewer"

$(PDF_DIR):
	@$(MKDIR) $(PDF_DIR)

pdf-clean: ## Removes PDF generated directory
	@$(call CLEANUP,$(NAME),,$(PDF_DIR),PDF files removed.,No PDF files to remove.)

.PHONY: pdf pdf-clean
