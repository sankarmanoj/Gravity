CC=g++
FRAMEWORK = OpenGL
OUTPUT = myrun
CUDAOUT = simCuda.o
LDFLAGS=-Wl,-rpath,/usr/local/cuda/lib
all:  cuda build

run: cuda build
	export LD_LIBRARY_PATH=/usr/local/cuda/lib:$(LD_LIBRARY_PATH)
	exec ./$(OUTPUT)

build :
		$(CC) $(CUDAOUT)  sim.cpp  $(LDFLAGS) -L/usr/local/cuda/lib -l cuda  -l cudart -l glfw3 -l GLEW -framework $(FRAMEWORK) -o $(OUTPUT)

cuda :
	nvcc -c simCuda.cu -o $(CUDAOUT)
clean :
	rm simCuda.o
	rm $(OUTPUT)
