all: build

CC = gcc

build:
	@echo "Building project..."
	$(CC) -std=c90 -lm -Wall ./main.c -o $(target)

clean:
	@echo "Cleaning up..."
	mkdir -p $(target)
	rm -rf $(target)/*