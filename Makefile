CC ?= gcc
CFLAGS ?= -O2 -Wall -Wextra -Iinclude -std=c11
LDLIBS ?= -lm
export TMPDIR ?= $(CURDIR)
OPENMP ?= 1
ifeq ($(OPENMP),1)
PARFLAGS := -fopenmp
endif
# Optional BLAS/LAPACK for wide supernodes (dgemm, dpotrf+dtrsm, dtrsv+dgemv):
#   make BLAS=1 BLAS_LIBS='-lopenblas'
# Runtime: VSDLSS_BLAS_MIN / VSDLSS_BLAS_SOLVE_MIN (panel width, default 32,
# 0 disables).  The BLAS must be thread safe and single threaded inside the
# solver (e.g. OpenBLAS with OPENBLAS_NUM_THREADS=1 and OMP_NUM_THREADS=1, or
# MKL sequential); results then match for every solver thread count but are
# not bitwise equal to the built-in kernels.
BLAS ?= 0
BLAS_LIBS ?= -lopenblas
ifeq ($(BLAS),1)
PARFLAGS += -DVSDLSS_BLAS
LDLIBS += $(BLAS_LIBS)
endif
# Optional METIS nested dissection for the M3 core (ordering 6):
#   make METIS=1 METIS_CFLAGS='-I<metis>/include' METIS_LIBS='-L<metis>/lib -lmetis'
# METIS must be built with 64-bit idx_t.  Stock METIS 5.1 keeps its random
# state in globals, so calls from concurrent components are serialized; set
# METIS_THREADSAFE=1 for a METIS built with -DUSE_GKRAND and thread-local
# GKlib random state, which lets VDD/GND orderings run concurrently.
METIS ?= 0
METIS_CFLAGS ?=
METIS_LIBS ?= -lmetis
METIS_THREADSAFE ?= 0
ifeq ($(METIS),1)
PARFLAGS += -DVSDLSS_METIS $(METIS_CFLAGS)
LDLIBS += $(METIS_LIBS)
ifeq ($(METIS_THREADSAFE),1)
PARFLAGS += -DVSDLSS_METIS_THREADSAFE
endif
endif

LIBSRCS := src/vsdlss.c src/vsdlss_status.c src/vsdlss_matrix.c \
           src/vsdlss_graph.c src/vsdlss_min_degree.c src/vsdlss_amd.c src/vsdlss_mld_graph.c \
           src/vsdlss_mld_partition.c src/vsdlss_mld.c src/vsdlss_ordering.c \
           src/vsdlss_factor.c src/vsdlss_io.c src/vsdlss_text_io.c src/vsdlss_components.c \
           src/vsdlss_reduction.c src/vsdlss_supernodal_symbolic.c \
           src/vsdlss_supernodal_numeric.c src/vsdlss_sn_reorder.c src/vsdlss_m3.c src/vsdlss_m4.c \
           src/vsdlss_panel.c src/vsdlss_m4_panel.c src/vsdlss_parallel.c \
           src/vsdlss_dense.c src/vsdlss_simd.c
LIBOBJS := $(LIBSRCS:.c=.o)

.PHONY: all test test-unit test-io test-ordering test-mld test-m3 test-m4 test-m4-panels test-m5 test-amd test-kernels test-parallel bench-parallel bench-m3 bench-sn sanitizers clean

all: vsdlss_solve vsdlss_solver

vsdlss_solve: src/main.o $(LIBOBJS)
	$(CC) $(CFLAGS) $(PARFLAGS) -o $@ $^ $(LDLIBS)

vsdlss_solver: src/main_vsdlss.o $(LIBOBJS)
	$(CC) $(CFLAGS) $(PARFLAGS) -o $@ $^ $(LDLIBS)

test_solver: test/test_solver.c $(LIBSRCS) include/vsdlss.h
	$(CC) $(CFLAGS) $(PARFLAGS) -o $@ test/test_solver.c $(LIBSRCS) $(LDLIBS)

test_io: test/test_io.c $(LIBSRCS) include/vsdlss.h
	$(CC) $(CFLAGS) $(PARFLAGS) -o $@ test/test_io.c $(LIBSRCS) $(LDLIBS)

test_ordering: test/test_ordering.c $(LIBSRCS) include/vsdlss.h src/vsdlss_internal.h
	$(CC) $(CFLAGS) $(PARFLAGS) -o $@ test/test_ordering.c $(LIBSRCS) $(LDLIBS)

test-unit: test_solver
	./test_solver

test-io: test_io
	./test_io

.PHONY: test-text
test-text: vsdlss_solver
	python3 test/test_text_dump.py

test-ordering: test_ordering
	./test_ordering

test-mld: test_mld
	./test_mld

test_mld: test/test_mld.c $(LIBSRCS) include/vsdlss.h src/vsdlss_internal.h
	$(CC) $(CFLAGS) $(PARFLAGS) -o $@ test/test_mld.c $(LIBSRCS) $(LDLIBS)

test_m3: test/test_m3.c test/m3_test_alloc.c test/m3_test_alloc.h $(LIBSRCS) include/vsdlss.h src/vsdlss_m3_internal.h
	$(CC) $(CFLAGS) $(PARFLAGS) -o $@ test/test_m3.c test/m3_test_alloc.c $(LIBSRCS) $(LDLIBS) \
	  -Wl,--wrap=malloc -Wl,--wrap=calloc -Wl,--wrap=realloc -Wl,--wrap=free

test-m3: test_m3
	./test_m3

test: all test-unit test-io test-text test-ordering test-mld test-m3 test-m4 test-m4-panels test-m5 test-amd test-kernels test-parallel test-supernodal
	python3 test/gen_sparse.py test_sparse 10
	./vsdlss_solver test_sparse
	./vsdlss_solver --disk-budget 8192 test_sparse

sanitizers:
	$(MAKE) clean
	ASAN_OPTIONS=detect_leaks=0 $(MAKE) \
	  CFLAGS='-O1 -g -Wall -Wextra -Werror -Iinclude -std=c11 -fsanitize=address,undefined -fno-omit-frame-pointer' \
	  LDLIBS='test/sanitizer_options.c -lm -fsanitize=address,undefined' test-unit test-io test-ordering test-mld test-m3 test-m4 test-m4-panels test-amd test-kernels test-parallel test-small test-reduced-dag test-supernodal

src/%.o: src/%.c include/vsdlss.h src/vsdlss_text_io.h src/vsdlss_internal.h src/vsdlss_m3_internal.h src/vsdlss_m4_internal.h src/vsdlss_parallel.h src/vsdlss_dense.h
	$(CC) $(CFLAGS) $(PARFLAGS) -c -o $@ $<

clean:
	rm -f libvsdlss.a quickstart vsdlss_solve vsdlss_solver test_solver test_io test_ordering test_mld test_m3 test_m4 test_m4_panels test_amd test_kernels test_parallel test_small test_reduced_dag test_supernodal bench_powergrid bench_pg_profile bench_parallel bench_m3 bench_sn bench_pg_solve bench_ibmpg bench_dense bench_dense_solve bench_cholmod bench_pardiso test_simd test_omp_tsan_probe src/*.o test_sparse.*

test_m4: test/test_m4.c $(LIBSRCS) include/vsdlss.h src/vsdlss_m4_internal.h src/vsdlss_parallel.h
	$(CC) $(CFLAGS) $(PARFLAGS) -o $@ test/test_m4.c $(LIBSRCS) $(LDLIBS)

test-m4: test_m4
	./test_m4

test_m4_panels: test/test_m4_panels.c $(LIBSRCS) include/vsdlss.h src/vsdlss_m4_internal.h src/vsdlss_parallel.h
	$(CC) $(CFLAGS) $(PARFLAGS) -o $@ test/test_m4_panels.c $(LIBSRCS) $(LDLIBS)

test-m4-panels: test_m4_panels
	./test_m4_panels

test-m5: vsdlss_solver
	python3 test/test_m5.py

test_amd: test/test_amd.c $(LIBSRCS) include/vsdlss.h src/vsdlss_internal.h src/vsdlss_m3_internal.h
	$(CC) $(CFLAGS) $(PARFLAGS) -o $@ test/test_amd.c $(LIBSRCS) $(LDLIBS)

test-amd: test_amd
	./test_amd

test_kernels: test/test_kernels.c $(LIBSRCS) include/vsdlss.h src/vsdlss_m3_internal.h
	$(CC) $(CFLAGS) $(PARFLAGS) -o $@ test/test_kernels.c $(LIBSRCS) $(LDLIBS)

test-kernels: test_kernels
	./test_kernels

test_parallel: test/test_parallel.c $(LIBSRCS) include/vsdlss.h src/vsdlss_parallel.h
	$(CC) $(CFLAGS) $(PARFLAGS) -o $@ test/test_parallel.c $(LIBSRCS) $(LDLIBS)

test-parallel: test_parallel
	./test_parallel

bench_parallel: test/bench_parallel.c $(LIBSRCS) include/vsdlss.h src/vsdlss_parallel.h
	$(CC) $(CFLAGS) $(PARFLAGS) -o $@ test/bench_parallel.c $(LIBSRCS) $(LDLIBS)

bench-parallel: bench_parallel
	./bench_parallel

bench_m3: test/bench_m3.c $(LIBSRCS) include/vsdlss.h src/vsdlss_internal.h src/vsdlss_m3_internal.h
	$(CC) $(CFLAGS) $(PARFLAGS) -o $@ test/bench_m3.c $(LIBSRCS) $(LDLIBS)

bench-m3: bench_m3
	./bench_m3

# Scaling on one connected component: component-level parallelism gives
# nothing here, so every speedup comes from the numeric and symbolic kernels.
# Arguments: grid side, max threads, RHS count.
bench_sn: test/bench_sn.c $(LIBSRCS) include/vsdlss.h src/vsdlss_m3_internal.h
	$(CC) $(CFLAGS) $(PARFLAGS) -o $@ test/bench_sn.c $(LIBSRCS) $(LDLIBS)

bench-sn: bench_sn
	./bench_sn 18 4 8

# Generated kernel changes must invalidate all consumers.
$(LIBOBJS) test_solver test_io test_ordering test_mld test_m3 test_m4 test_m4_panels test_amd test_kernels test_parallel bench_parallel bench_m3 bench_sn: src/vsdlss_small_solve.inc

test_small: test/test_small.c $(LIBSRCS) src/vsdlss_small_solve.inc
	$(CC) $(CFLAGS) $(PARFLAGS) -o $@ test/test_small.c $(LIBSRCS) $(LDLIBS)

test-small: test_small
	./test_small

test: test-small

test_reduced_dag: test/test_reduced_dag.c $(LIBSRCS) src/vsdlss_small_solve.inc
	$(CC) $(CFLAGS) $(PARFLAGS) -o $@ test/test_reduced_dag.c $(LIBSRCS) $(LDLIBS)

test-reduced-dag: test_reduced_dag
	./test_reduced_dag

test: test-reduced-dag

AR ?= ar
.PHONY: smoke dist test-small test-reduced-dag
libvsdlss.a: $(LIBOBJS)
	$(AR) rcs $@ $^

quickstart: examples/quickstart.c libvsdlss.a include/vsdlss.h
	$(CC) $(CFLAGS) $(PARFLAGS) -o $@ examples/quickstart.c libvsdlss.a $(LDLIBS)

smoke: quickstart
	./quickstart m1
	./quickstart m3
	./quickstart m4
	./quickstart m4-reduced

dist:
	python3 tools/package_source.py

test_supernodal: test/test_supernodal.c $(LIBSRCS) include/vsdlss.h src/vsdlss_m3_internal.h src/vsdlss_dense.h
	$(CC) $(CFLAGS) $(PARFLAGS) -o $@ test/test_supernodal.c $(LIBSRCS) $(LDLIBS)

test-supernodal: test_supernodal
	./test_supernodal

.PHONY: test-supernodal

bench_powergrid: test/bench_powergrid.c $(LIBSRCS) include/vsdlss.h src/vsdlss_m3_internal.h
	$(CC) $(CFLAGS) $(PARFLAGS) -o $@ test/bench_powergrid.c $(LIBSRCS) $(LDLIBS)

# 20M full nonzeros, degree mix 25/50/20/5 (%), AMD, 2 threads, 4 RHS
bench-powergrid: bench_powergrid
	./bench_powergrid 2e7 5 2 4

.PHONY: bench-powergrid

bench_pg_profile: test/bench_pg_profile.c $(LIBSRCS) include/vsdlss.h src/vsdlss_m3_internal.h
	$(CC) $(CFLAGS) $(PARFLAGS) -o $@ test/bench_pg_profile.c $(LIBSRCS) $(LDLIBS)

# Customer degree profile, 22,875,397 nodes (needs ~6 GB RAM), AMD, 2 threads
bench-pg-profile: bench_pg_profile
	VSDLSS_TRACE=1 ./bench_pg_profile 5 2 1 1

# Same histogram as two separate nets (VDD 52% / GND 48%): two components
bench-pg-vddgnd: bench_pg_profile
	PG_NETS=2 VSDLSS_TRACE=1 ./bench_pg_profile 5 2 1 1

.PHONY: bench-pg-profile bench-pg-vddgnd

# Single-solve scaling (one factor, several solve thread counts) and the
# internal-order solve; PG_DUMP=file exports the system for bench_cholmod.
bench_pg_solve: test/bench_pg_solve.c $(LIBSRCS) include/vsdlss.h src/vsdlss_m3_internal.h
	$(CC) $(CFLAGS) $(PARFLAGS) -o $@ test/bench_pg_solve.c $(LIBSRCS) $(LDLIBS)

bench-pg-solve: bench_pg_solve
	PG_NETS=2 SOLVE_THREADS="1 2 4" ./bench_pg_solve 5 2 1 1

# IBM power grid benchmarks (ibmpg1-6); netlists are downloaded separately.
bench_ibmpg: test/bench_ibmpg.c $(LIBSRCS) include/vsdlss.h src/vsdlss_m3_internal.h
	$(CC) $(CFLAGS) $(PARFLAGS) -o $@ test/bench_ibmpg.c $(LIBSRCS) $(LDLIBS)

# Dense kernels against BLAS/LAPACK (needs BLAS=1).
bench_dense: test/bench_dense.c src/vsdlss_dense.c src/vsdlss_parallel.c src/vsdlss_status.c src/vsdlss_dense.h
	$(CC) $(CFLAGS) $(PARFLAGS) -o $@ test/bench_dense.c src/vsdlss_dense.c src/vsdlss_parallel.c src/vsdlss_status.c $(LDLIBS)

# CHOLMOD on a PG_DUMP file: make bench_cholmod CHOLMOD_CFLAGS='-I...' CHOLMOD_LIBS='-lcholmod ...'
CHOLMOD_CFLAGS ?=
CHOLMOD_LIBS ?= -lcholmod -lamd -lcamd -lcolamd -lccolamd -lsuitesparseconfig
bench_cholmod: test/bench_cholmod.c
	$(CC) -O2 $(PARFLAGS) $(CHOLMOD_CFLAGS) -o $@ test/bench_cholmod.c $(CHOLMOD_LIBS) $(BLAS_LIBS) -lm

.PHONY: bench-pg-solve

# MKL PARDISO on a PG_DUMP file:
#   make bench_pardiso PARDISO_LIBS='-L<mkl>/lib -lmkl_rt -Wl,--no-as-needed -lgomp -lm'
PARDISO_LIBS ?= -lmkl_rt -Wl,--no-as-needed -lgomp -lm -ldl -lpthread
bench_pardiso: test/bench_pardiso.c
	$(CC) -O2 -std=c11 -fopenmp -o $@ test/bench_pardiso.c $(PARDISO_LIBS)

test_simd: test/test_simd.c src/vsdlss_simd.c src/vsdlss_simd.h include/vsdlss.h
	$(CC) $(CFLAGS) -o $@ test/test_simd.c src/vsdlss_simd.c

.PHONY: test-simd
test-simd: test_simd
	./test_simd

test: test-simd

# Single-RHS forward/back substitution only. Optional CHOLMOD dependency.
# The VSDLSS side is ALWAYS built-in: do not pass BLAS=1 for this target.
DENSE_SOLVE_BENCH_SRCS := src/vsdlss_panel.c src/vsdlss_dense.c src/vsdlss_parallel.c src/vsdlss_simd.c src/vsdlss_status.c
bench_dense_solve: test/bench_dense_solve.c $(DENSE_SOLVE_BENCH_SRCS) src/vsdlss_small_solve.inc src/vsdlss_simd.h src/vsdlss_dense.h src/vsdlss_m3_internal.h include/vsdlss.h
	$(CC) $(CFLAGS) $(filter-out -DVSDLSS_BLAS,$(PARFLAGS)) $(CHOLMOD_CFLAGS) -o $@ test/bench_dense_solve.c $(DENSE_SOLVE_BENCH_SRCS) $(CHOLMOD_LIBS) $(BLAS_LIBS) -lm
