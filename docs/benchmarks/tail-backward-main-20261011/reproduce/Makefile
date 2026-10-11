# Invoke after the repository Makefile, from the repository root.
# These targets are diagnostic only; no production target is changed.
TAIL_DOC_DIR := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))
TAIL_OUT ?= /tmp/vsdlss-tail-research

$(TAIL_OUT)/analysis_probe: $(TAIL_DOC_DIR)/analysis_probe.c $(LIBSRCS)
	@mkdir -p $(TAIL_OUT)
	$(CC) $(CFLAGS) $(PARFLAGS) -o $@ $< $(LIBSRCS) $(LDLIBS)

$(TAIL_OUT)/generate_wires: $(TAIL_DOC_DIR)/generate_wires.c $(LIBSRCS)
	@mkdir -p $(TAIL_OUT)
	$(CC) $(CFLAGS) $(PARFLAGS) -o $@ $< $(LIBSRCS) $(LDLIBS)

.PHONY: tail-research-build
tail-research-build: $(TAIL_OUT)/analysis_probe $(TAIL_OUT)/generate_wires
