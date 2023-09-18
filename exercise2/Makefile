### MKL libraries
###
###
MKL= -L${MKLROOT}/lib/intel64  -lmkl_intel_lp64 -lmkl_gnu_thread -lmkl_core -lgomp -lpthread -lm -ldl

### OpenBLAS libraries 
OPENBLASROOT=${OPENBLAS_ROOT}

cpu: sgemm_mkl.x dgemm_mkl.x sgemm_oblas.x dgemm_oblas.x


sgemm_mkl.x: my_gemm.c
	gcc -DUSE_FLOAT -DMKL $^ -m64 -I${MKLROOT}/include $(MKL)  -o $@

dgemm_mkl.x: my_gemm.c
	gcc -DUSE_DOUBLE -DMKL $^ -m64 -I${MKLROOT}/include $(MKL)  -o $@

sgemm_oblas.x: my_gemm.c
	gcc -DUSE_FLOAT -DOPENBLAS $^ -m64 -I${OPENBLASROOT}/include -L/${OPENBLASROOT}/lib -lopenblas -lpthread -o $@ -fopenmp

dgemm_oblas.x: my_gemm.c
	gcc -DUSE_DOUBLE -DOPENBLAS $^ -m64 -I${OPENBLASROOT}/include -L/${OPENBLASROOT}/lib -lopenblas -lpthread -o $@ -fopenmp


clean:
	rm -rf *.x
