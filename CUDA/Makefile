CC=g++
FRAMEWORK = OpenGL
OUTPUT = run
all:



run: build
	./$(OUTPUT)

build :
		$(CC) sim.cpp -Wall -l glfw3 -l GLEW -framework $(FRAMEWORK) -o $(OUTPUT)
