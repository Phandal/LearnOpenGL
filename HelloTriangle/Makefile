all: src/main.c
	cc -Wall -Wextra -I./deps/GLAD/include -o build/prog src/*.c -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit

.PHONY: clean
clean:
	-@rm ./build/*
