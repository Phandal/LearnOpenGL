SRC = src/*.c

all: src/main.c src/glad.c src/shader.h
	cc -Wall -Wextra -I./deps/GLAD/include -I./deps/CGLM/include -DDEBUG $(SRC) -g -lglfw3 -framework OpenGL -framework Cocoa -framework IOKit -o build/prog

linux: src/main.c src/glad.c src/shader.h
	cc -I/usr/local/include -I./deps/GLAD/include -I./deps/CGLM/include -DDEBUG -o build/prog $(SRC) -L/usr/local/lib -lglfw3 -lrt -lm -ldl -lX11 -lpthread -lxcb -lXau -lXdmcp

.PHONY: clean
clean:
	-@rm -rf ./build/*
