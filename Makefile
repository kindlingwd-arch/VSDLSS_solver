# VSDLSS-style Very Sparse Direct Linear System Solver
CC      ?= gcc
CFLAGS  ?= -O2 -Wall -Wextra -Iinclude -std=gnu11
LDLIBS  ?= -lm

TARGET  := vsdlss_solve
SOLVER  := vsdlss_solver      # catalog-based reproduced solver (file entry)
HARNESS := vsdlss_harness     # checks .hdr/.mat*/.rhs
DSTEST  := vsdlss_ds_test
SRCS    := src/main.c src/vsdlss.c
APISRCS := src/main_vsdlss.c src/vsdlss_catalog.c src/vsdlss_ds.c src/vsdlss.c
HSRCS   := src/vsdlss_harness.c src/vsdlss.c
DSSRCS  := src/ds_test.c src/vsdlss_ds.c
HDRS    := include/vsdlss.h include/vsdlss_api.h include/vsdlss_catalog.h include/vsdlss_ds.h
OBJS    := $(SRCS:.c=.o)
AOBJS   := $(APISRCS:.c=.o)
HOBJS   := $(HSRCS:.c=.o)
DSOBJS  := $(DSSRCS:.c=.o)

.PHONY: all solver harness ds test clean run

all: $(TARGET)

solver: $(SOLVER)

harness: $(HARNESS)

$(TARGET): $(OBJS) $(HDRS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDLIBS)

$(SOLVER): $(AOBJS) $(HDRS)
	$(CC) $(CFLAGS) -o $@ $(AOBJS) $(LDLIBS)

$(HARNESS): $(HOBJS) $(HDRS)
	$(CC) $(CFLAGS) -o $@ $(HOBJS) $(LDLIBS)

$(DSTEST): $(DSOBJS) $(HDRS)
	$(CC) $(CFLAGS) -o $@ $(DSOBJS) $(LDLIBS)

src/%.o: src/%.c $(HDRS)
	$(CC) $(CFLAGS) -c -o $@ $<

ds: $(DSTEST)
	./$(DSTEST)

test: $(SOLVER)
	python3 test/gen_sparse.py test_sparse 10
	./$(SOLVER) test_sparse
	./$(HARNESS) test_sparse 1

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(SOLVER) $(HARNESS) $(OBJS) $(AOBJS) $(HOBJS) \
	      test_sparse.* test_sparse500.* test_sparse2k.*
