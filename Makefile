CC ?= gcc
CFLAGS ?= -O2 -Wall -Wextra -Iinclude -std=c11
LDLIBS ?= -lm

LIBSRCS := src/vsdlss.c src/vsdlss_status.c src/vsdlss_matrix.c \
           src/vsdlss_graph.c src/vsdlss_min_degree.c src/vsdlss_mld_graph.c \
           src/vsdlss_mld_partition.c src/vsdlss_mld.c src/vsdlss_ordering.c \
           src/vsdlss_factor.c src/vsdlss_io.c src/vsdlss_components.c \
           src/vsdlss_reduction.c src/vsdlss_supernodal_symbolic.c
LIBOBJS := $(LIBSRCS:.c=.o)

.PHONY: all test test-unit test-io test-ordering test-mld test-m3 sanitizers clean

all: vsdlss_solve vsdlss_solver

vsdlss_solve: src/main.o $(LIBOBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

vsdlss_solver: src/main_vsdlss.o $(LIBOBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDLIBS)

test_solver: test/test_solver.c $(LIBSRCS) include/vsdlss.h
	$(CC) $(CFLAGS) -o $@ test/test_solver.c $(LIBSRCS) $(LDLIBS)

test_io: test/test_io.c $(LIBSRCS) include/vsdlss.h
	$(CC) $(CFLAGS) -o $@ test/test_io.c $(LIBSRCS) $(LDLIBS)

test_ordering: test/test_ordering.c $(LIBSRCS) include/vsdlss.h src/vsdlss_internal.h
	$(CC) $(CFLAGS) -o $@ test/test_ordering.c $(LIBSRCS) $(LDLIBS)

test-unit: test_solver
	./test_solver

test-io: test_io
	./test_io

test-ordering: test_ordering
	./test_ordering

test-mld: test_mld
	./test_mld

test_mld: test/test_mld.c $(LIBSRCS) include/vsdlss.h src/vsdlss_internal.h
	$(CC) $(CFLAGS) -o $@ test/test_mld.c $(LIBSRCS) $(LDLIBS)

test_m3: test/test_m3.c $(LIBSRCS) include/vsdlss.h src/vsdlss_m3_internal.h
	$(CC) $(CFLAGS) -o $@ test/test_m3.c $(LIBSRCS) $(LDLIBS)

test-m3: test_m3
	./test_m3

test: all test-unit test-io test-ordering test-mld
	python3 test/gen_sparse.py test_sparse 10
	./vsdlss_solver test_sparse

sanitizers:
	$(MAKE) clean
	ASAN_OPTIONS=detect_leaks=0 $(MAKE) \
	  CFLAGS='-O1 -g -Wall -Wextra -Werror -Iinclude -std=c11 -fsanitize=address,undefined -fno-omit-frame-pointer' \
	  LDLIBS='-lm -fsanitize=address,undefined' test-unit test-io test-ordering test-mld

src/%.o: src/%.c include/vsdlss.h src/vsdlss_internal.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f vsdlss_solve vsdlss_solver test_solver test_io test_ordering test_mld test_m3 src/*.o test_sparse.*
