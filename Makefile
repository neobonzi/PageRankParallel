CPP=icc
CUDACC=nvcc
CFLAGS=-O3
NVFLAGS=-O3 -g -ccbin g++ -m64 -gencode arch=compute_20,code=sm_20 -gencode arch=compute_30,code=sm_30 -gencode arch=compute_35,code=sm_35 -gencode arch=compute_37,code=sm_37 -gencode arch=compute_50,code=sm_50 -gencode arch=compute_52,code=sm_52 -gencode arch=compute_52,code=compute_52 --x cu
CUDASRC=page_rank.cpp cuda_page_rank.cu
MICSRC=page_rank.cpp

all: xeon_phi cuda

xeon_phi: $(MICSRC)
	g++ $(CFLAGS) -o micrank $^

cuda: $(CUDASRC)
	nvcc $(NVFLAGS) -o cudarank $^
clean:
	rm -f *.o micrank cudarank 
