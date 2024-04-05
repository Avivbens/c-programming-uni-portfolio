all: build

CC = gcc
COMPILER = $(CC) -std=c90 -ansi -pedantic -Wall -Werror

build:
	@echo "Building project..."
	$(eval FILES := $(shell ls ./** | grep .c | sed 's/\.c//g'))
	$(shell for file in $(FILES); do $(COMPILER) $$file.c -o $(target)/$$file.o -c; done)

	make link

link:
	@echo "Linking project..."
	$(eval FILES := $(shell ls $(target) | grep .o))

	$(eval TARGET_NAME := $(shell echo "$(target)/RUNNER.o"))
	(cd $(target) && $(COMPILER) -o $(TARGET_NAME) $(FILES))

clean:
	@echo "Cleaning up..."
	mkdir -p $(target)
	rm -rf $(target)/*