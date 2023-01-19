SRC = src/*.c

all: src/main.c src/glad.c src/shader.h
	cc -Wall -Wextra -I./deps/GLAD/include $(SRC) -g -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -o build/prog

.PHONY: clean
clean:
	-@rm -rf ./build/*
