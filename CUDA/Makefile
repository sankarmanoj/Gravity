CC=g++
FRAMEWORK = OpenGL
OUTPUT = run
all:  cuda build

run: all
	./$(OUTPUT)

build :
		$(CC) sim.cpp   -L/usr/local/cuda/lib -L.   -l glfw3 -l GLEW -framework $(FRAMEWORK)   -o $(OUTPUT)

cuda :
	nvcc  -c -Xcompiler -fPIC -shared -G simCuda.cu -o libcudasim.so
