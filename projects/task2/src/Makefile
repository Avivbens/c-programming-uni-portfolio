all: clean build link

CC = gcc
COMPILER = $(CC) -std=c90 -ansi -pedantic -Wall -Werror

build:
	@echo "Building project..."

	# Create a variable holding all C files under the target, without file extension
	$(eval FILES := $(shell find . -type f -name '*.c' | sed 's/\.c//g'))
	@echo "Files: $(FILES)"

	$(eval DIRECTORIES_TO_CREATE := $(shell find . -type d))
	@echo "dirs: $(DIRECTORIES_TO_CREATE)"

	@echo "Creating directories..."
	$(shell for directory in $(DIRECTORIES_TO_CREATE); do mkdir -p $(target)/$$directory; done)

	@echo "Compiling files..."
	$(shell for file in $(FILES); do $(COMPILER) $$file.c -o $(target)/$$file.o -c; done)


	@echo "\n----- Done Building -----\n"

link:
	@echo "Linking project..."

	$(eval FILES := $(shell find $(target) -type f -name '*.o'))
	@echo "Files: $(FILES)"

	$(eval TARGET_NAME := $(shell echo "$(target)/RUNNER.o"))
	$(COMPILER) -o $(TARGET_NAME) $(FILES)

	@echo "\n----- Done Linking -----\n"

clean:
	@echo "Cleaning up..."

	mkdir -p $(target)
	rm -rf $(target)/*

	@echo "\n----- Done Cleaning -----\n"
