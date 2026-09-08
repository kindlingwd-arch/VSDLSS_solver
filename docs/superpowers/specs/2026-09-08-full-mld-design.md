# Full Deterministic MLD Design

## Goal and evidence boundary

Replace the M2 BFS-only separator baseline with an independently written deterministic implementation of the recovered MLD pipeline: weighted strong-edge matching, coarse graph construction, multistart initial bisection, balance/FM refinement, level-by-level projection, edge-separator to node-separator conversion, and recursive nested-dissection ordering. The recovered functions establish these stages, but not stable layouts or every constant; therefore this implementation preserves algorithmic responsibilities and invariants rather than proprietary byte layout or random streams.

## Graph hierarchy

Each immutable level stores symmetric weighted CSR (`offset`, `neighbor`, `edge_weight`), positive `vertex_weight`, stable representative IDs, and (except the coarsest level) `fine_to_coarse`. The input graph begins with unit vertex and edge weights. Duplicate coarse edges are summed and internal aggregate edges are discarded.

## Coarsening

Stable increasing `(degree, representative)` order drives SHEM. Each unmatched vertex chooses the unmatched adjacent vertex with greatest edge weight whose aggregate weight does not exceed `max(2, ceil(3*total_weight/coarse_target))`; ties use the smaller representative. Unmatched vertices become singleton aggregates. Stop when the graph has at most 32 vertices, contraction is below 15 percent, or undirected density exceeds 0.20.

## Partition and refinement

The coarsest graph receives deterministic multistart region-growing bisections seeded by minimum representative, minimum weighted degree, maximum weighted degree, and stable quartile representatives. Growth selects frontier vertices minimizing added cut while approaching half total vertex weight. Every candidate is balanced to a 7/8 maximum side weight and refined by deterministic FM passes. An FM pass computes gain `external-internal`, permits moves that preserve the balance bound, records moves, and commits only the best positive cumulative prefix.

On uncoarsening, coarse labels are projected through `fine_to_coarse`; balance and FM are rerun at every finer level.

## Node separator and recursion

Cross-partition boundary edges form a bipartite graph. Deterministic augmenting-path maximum matching followed by Konig alternating reachability constructs a minimum vertex cover; cover vertices become the node separator. Removing the separator must leave no left/right edge. Empty-side or empty-separator degeneracy falls back to the verified M2 separator routine.

Connected components are processed in stable representative order. Components of at most 32 vertices use dynamic minimum degree. Larger components recursively emit left, right, then a minimum-degree ordering of the separator.

## Interfaces and acceptance

`vsdlss_mld_top_partition` keeps its existing contract. Internal diagnostic `vsdlss_mld_hierarchy_analyze` reports levels, first contraction, initial/final cut, and imbalance for tests. Successful results are deterministic valid permutations. Tests independently verify weighted matching/coarse aggregation, cut non-increase under refinement, projection, minimum-cover separation, solve correctness, and prediction/actual factor agreement. Strict C11, `-Wall -Wextra -Werror`, ASan and UBSan remain required.
