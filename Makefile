CPP=icc
CUDACC=nvcc
CFLAGS=-g -O3 -qopt-report=3 -restrict -g -mkl -debug inline-debug-info -qopenmp
NVFLAGS=-O3 -g -ccbin g++ -m64 -gencode arch=compute_20,code=sm_20 -gencode arch=compute_30,code=sm_30 -gencode arch=compute_35,code=sm_35 -gencode arch=compute_37,code=sm_37 -gencode arch=compute_50,code=sm_50 -gencode arch=compute_52,code=sm_52 -gencode arch=compute_52,code=compute_52 --x cu -lcublas
CUDASRC=page_rank.cpp cuda_page_rank.cu cuda_graph_utils.cpp
MICSRC=page_rank.cpp phi_page_rank.cpp phi_graph_utils.cpp

all: xeon_phi cuda

xeon_phi: $(MICSRC)
	$(CPP) $(CFLAGS) -o micrank $^

cuda: $(CUDASRC)
	$(CUDACC) $(NVFLAGS) -o cudarank $^
clean:
	rm -f *.o micrank cudarank 
