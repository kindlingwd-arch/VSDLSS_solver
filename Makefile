CC ?= gcc
CFLAGS ?= -O2 -Wall -Wextra -Iinclude -std=c11
LDLIBS ?= -lm
export TMPDIR ?= $(CURDIR)
OPENMP ?= 1
ifeq ($(OPENMP),1)
PARFLAGS := -fopenmp
endif

LIBSRCS := src/vsdlss.c src/vsdlss_status.c src/vsdlss_matrix.c \
           src/vsdlss_graph.c src/vsdlss_min_degree.c src/vsdlss_mld_graph.c \
           src/vsdlss_mld_partition.c src/vsdlss_mld.c src/vsdlss_ordering.c \
           src/vsdlss_factor.c src/vsdlss_io.c src/vsdlss_components.c \
           src/vsdlss_reduction.c src/vsdlss_supernodal_symbolic.c \
           src/vsdlss_supernodal_numeric.c src/vsdlss_m3.c src/vsdlss_m4.c \
           src/vsdlss_panel.c src/vsdlss_m4_panel.c src/vsdlss_parallel.c
LIBOBJS := $(LIBSRCS:.c=.o)

.PHONY: all test test-unit test-io test-ordering test-mld test-m3 test-m4 test-m4-panels test-m5 test-parallel bench-parallel bench-m3 sanitizers clean

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

test: all test-unit test-io test-ordering test-mld test-m3 test-m4 test-m4-panels test-m5 test-parallel
	python3 test/gen_sparse.py test_sparse 10
	./vsdlss_solver test_sparse
	./vsdlss_solver --disk-budget 8192 test_sparse

sanitizers:
	$(MAKE) clean
	ASAN_OPTIONS=detect_leaks=0 $(MAKE) \
	  CFLAGS='-O1 -g -Wall -Wextra -Werror -Iinclude -std=c11 -fsanitize=address,undefined -fno-omit-frame-pointer' \
	  LDLIBS='test/sanitizer_options.c -lm -fsanitize=address,undefined' test-unit test-io test-ordering test-mld test-m3 test-m4 test-m4-panels test-parallel

src/%.o: src/%.c include/vsdlss.h src/vsdlss_internal.h src/vsdlss_m3_internal.h src/vsdlss_m4_internal.h src/vsdlss_parallel.h
	$(CC) $(CFLAGS) $(PARFLAGS) -c -o $@ $<

clean:
	rm -f vsdlss_solve vsdlss_solver test_solver test_io test_ordering test_mld test_m3 test_m4 test_m4_panels test_parallel bench_parallel bench_m3 test_omp_tsan_probe src/*.o test_sparse.*

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
