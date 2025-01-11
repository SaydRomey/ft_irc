
# ==============================
##@ 🤖 Class creator
# ==============================

class: ## Prompt assisted class creation
	@echo "Enter the class name: "; \
	read classname; \
	classname_upper=`echo $$classname | tr a-z A-Z`; \
	\
	# Prompt for .hpp directory \
	read -p "Use default header directory '$(INC_DIR)'? [y/Y] for yes, or enter a custom directory: " header_dir; \
	if [ "$$header_dir" = "y" ] || [ "$$header_dir" = "Y" ] || [ -z "$$header_dir" ]; then \
		header_dir="$(INC_DIR)"; \
	fi; \
	mkdir -p "$$header_dir"; \
	\
	# Prompt for .cpp directory \
	read -p "Use default source directory '$(SRC_DIR)'? [y/Y] for yes, or enter a custom directory: " source_dir; \
	if [ "$$source_dir" = "y" ] || [ "$$source_dir" = "Y" ] || [ -z "$$source_dir" ]; then \
		source_dir="$(SRC_DIR)"; \
	fi; \
	mkdir -p "$$source_dir"; \
	\
	# Check for file existence \
	if [ -f "$$header_dir/$$classname.hpp" ] || [ -f "$$source_dir/$$classname.cpp" ]; then \
		read -p "Files exist in selected directories. Overwrite? [y/N]: " confirm; \
		if [ "$$confirm" != "y" ] && [ "$$confirm" != "Y" ]; then \
			echo "Canceling class creation"; \
			exit 1; \
		fi; \
	fi; \
	\
	# Generate header and source files \
	echo "$$CLASS_HEADER" \
	| sed "s/CLASSNAME_UPPER/$$classname_upper/g" \
	| sed "s/CLASSNAME/$$classname/g" > "$$header_dir/$$classname.hpp"; \
	echo "$$CLASS_CPP" \
	| sed "s/CLASSNAME/$$classname/g" > "$$source_dir/$$classname.cpp"; \
	\
	echo "$(GREEN)Class '$$classname' created successfully!$(RESET)"; \
	echo "Header file: $(BOLD)$$header_dir/$$classname.hpp$(RESET)"; \
	echo "Source file: $(BOLD)$$source_dir/$$classname.cpp$(RESET)"

# **************************************************************************** # # .hpp template

define CLASS_HEADER

#ifndef CLASSNAME_UPPER_HPP
# define CLASSNAME_UPPER_HPP

class CLASSNAME
{
	public:
		CLASSNAME(void);
		CLASSNAME(const CLASSNAME &other);
		CLASSNAME&	operator=(const CLASSNAME &other);
		~CLASSNAME(void);
	
	private:

};

#endif // CLASSNAME_UPPER_HPP
endef

export CLASS_HEADER

# **************************************************************************** # # .cpp template

define CLASS_CPP

#include "CLASSNAME.hpp"

CLASSNAME::CLASSNAME(void) {}

CLASSNAME::~CLASSNAME(void) {}

CLASSNAME::CLASSNAME(const CLASSNAME &other)
{
	*this = other;
}

CLASSNAME&	CLASSNAME::operator=(const CLASSNAME &other)
{
	// if (this != &other)
	// {}
	
	return (*this);
}

endef

export CLASS_CPP

# **************************************************************************** #
