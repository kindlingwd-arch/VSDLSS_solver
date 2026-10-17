# METIS 5.1.0 (bundled)

METIS 5.1.0 by George Karypis, University of Minnesota, under the Apache
License 2.0 (`LICENSE.txt`).  Taken from the copy distributed with
SuiteSparse 5.x; only what the library build needs is kept: `include/`,
`GKlib/*.{c,h}`, `libmetis/*.{c,h}`.  The programs, test graphs, manual and
upstream build files are omitted.

Modifications for VSDLSS:

- `include/metis.h`: `IDXTYPEWIDTH 64` (as shipped by SuiteSparse); the
  solver's index type is 64-bit and `src/vsdlss_ordering.c` refuses to
  compile against a 32-bit `idx_t`.
- `GKlib/random.c`: the Mersenne Twister state (`mt[]`, `mti`) is
  `__thread`, so that with `-DUSE_GKRAND` concurrent `METIS_NodeND` calls
  (the VDD and GND nets are ordered at the same time) do not share random
  state.  Each call seeds its own thread's state, so orderings do not depend
  on which thread runs them.

Built by the top-level Makefile with `make METIS=1` (no `METIS_CFLAGS`):
objects go to `build/metis/`, flags
`-O3 -DNDEBUG -DNDEBUG2 -D_GNU_SOURCE -DLINUX -DUSE_GKRAND -std=gnu99
-fno-strict-aliasing -w`.
