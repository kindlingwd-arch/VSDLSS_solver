#ifndef VSDLSS_DS_H
#define VSDLSS_DS_H
/* VSDLSS data-structure utilities, reconstructed from the decompiled functions.
 * Standard, correct implementations of the containers the solver uses:
 * priority queues (Pq / Pqi / Bpq), stack / double-ended queue, char-ptr list,
 * graph edge list, mark types, hash table, adjacency graph, and a genuine
 * multilevel nested-dissection ordering (MLDOrder).  Handles are opaque
 * pointers returned as longlong. */

#include "vsdlss_catalog.h"
#include <stdio.h>

/* ---- priority queue: binary min-heap on node indices with a value ---- */
typedef struct pq { int n, max; int *nodes; double *val; } vsdlss_pq;
longlong newPq_vsdlss(int n);
longlong deletePq_vsdlss(longlong h);
void     buildPq_vsdlss(longlong h, int n);
void     insertPqInt_vsdlss(longlong h, int node, double v);
void     minPq_vsdlss(longlong h, unsigned int *node);
int      minPqCost_vsdlss(int node, int dim, longlong h);
void     removeFromPq_vsdlss(int node, longlong h);
void     updatePq_vsdlss(int node, int dim, longlong h);
void     nqPq_vsdlss(int node, undefined4 v, longlong h);
void     deqPq_vsdlss(longlong h, unsigned int *node);
void     flushPq_vsdlss(longlong h);
int      moreInPq_vsdlss(longlong h);
int      elemInPq_vsdlss(longlong h, int node, undefined4 *pos);
longlong pqElemsVec_vsdlss(longlong h);
void     pqSwap_vsdlss(int i, int j, longlong h);
void     pqDecreasedKeyIter_vsdlss(int pos, longlong h);
void     pqIncreasedKeyIter_vsdlss(int pos, longlong h);
void     dumpPq_vsdlss(longlong h);

/* ---- indexed priority queue (position array) ---- */
longlong newPqiHeaderOnly_vsdlss(int n);
longlong newPqiNoNewVecs_vsdlss(int n, undefined8 a, undefined8 b, undefined8 c);
longlong deletePqi_vsdlss(longlong h);
void     buildPqi_vsdlss(longlong h, int n);
int      minPqiCost_vsdlss(int node, int dim, longlong h);
int      minPqiCostNonLexicographic_vsdlss(int node, int dim, longlong h);
void     removeFromPqi_vsdlss(int node, longlong h);
void     updatePqi_vsdlss(int node, int dim, longlong h);
void     nqPqi_vsdlss(int node, undefined4 v, longlong h);
void     dqPqi_vsdlss(longlong h, unsigned int *node);
void     flushPqi_vsdlss(longlong h);
int      moreInPqi_vsdlss(longlong h);
void     pqiSwap_vsdlss(int i, int j, longlong h);
void     pqiDecreasedKeyIter_vsdlss(int pos, longlong h);
void     pqiIncreasedKeyIter_vsdlss(int pos, longlong h);
undefined8 setPqiToNullPqi_vsdlss(longlong h);
void     dumpPqi_vsdlss(longlong h);

/* ---- bucket priority queue (buckets by integer key) ---- */
longlong newBpq_vsdlss(int n);
longlong deleteBpq_vsdlss(longlong h);
void     initBpq_vsdlss(int n, undefined4 *arr);
void     buildBpq_vsdlss(int *perm, undefined8 a);
void     buildBpqArr_vsdlss(int *perm, undefined8 a);
int      getMinBucketBpq_vsdlss(longlong h);
void     remFromBucketBpq_vsdlss(int node, int *perm);
void     remFromBucketMarkInBpq_vsdlss(int node, unsigned int *perm);
void     nqBpq_vsdlss(int node, int key, int *perm);
undefined8 setBpqToNull_vsdlss(longlong h);

/* ---- stack / double-ended queue ---- */
longlong newStack_vsdlss(undefined4 sz);
int      emptyStack_vsdlss(longlong h);
longlong deleteStack_vsdlss(longlong h);
longlong newAStack_vsdlss(int sz);
int      emptyAStack_vsdlss(longlong h);
longlong deleteAStack_vsdlss(longlong h);
void     initAStack_vsdlss(longlong h);
void     pushAStack_vsdlss(longlong h, undefined4 v);
undefined4 popAStack_vsdlss(longlong h);
void     dumpAStack_vsdlss(longlong h);
longlong newDoubleEndedQ_vsdlss(undefined4 n);
longlong deleteDoubleEndedQ_vsdlss(longlong h);
void     nq_vsdlss(int *q, undefined4 v);
undefined4 dq_vsdlss(int *q);
int      emptyDeq_vsdlss(longlong h);
undefined4 remFromDeq_vsdlss(undefined4 *q);
void     nqLastDeq_vsdlss(undefined8 q, longlong h);

/* ---- char-pointer list (with chunks) ---- */
void    newCharPtrList_vsdlss(undefined8 p, undefined8 list);
longlong deleteCharPtrList_vsdlss(longlong list);
void    newCharPtrChunk_vsdlss(void);
undefined8 snipDispCharPtrListElem_vsdlss(longlong list, longlong pos);
undefined8 snipAndReturnCharPtrListElem_vsdlss(longlong list, longlong pos);

/* ---- graph edge list ---- */
void     newCEdgelist_vsdlss(undefined4 a, undefined8 b);
longlong deleteCEdgelist_vsdlss(longlong h);
int      getCEdgelistSize_vsdlss(longlong h);
longlong newCEdgeVector_vsdlss(int a, int b, undefined8 c);
void     newCEdgeChunk_vsdlss(void);
longlong copyArrayToCEdgelist_vsdlss(longlong arr, int n);
int      copyCEdgelistToArray_vsdlss(undefined4 *to, undefined4 *from);
undefined8 snipAndReturnCEdgelistElem_vsdlss(longlong h, longlong pos);
undefined8 snipDispCEdgelistElem_vsdlss(longlong h, longlong pos);
longlong deleteEntireCEdgelist_vsdlss(longlong h);

/* ---- list marks / counting marks ---- */
longlong newListMark_vsdlss(int n);
longlong deleteListMark_vsdlss(longlong h);
void     markListMark_vsdlss(int *m, int i);
void     unmarkListMark_vsdlss(longlong h);
void     unmarkListMarkReturnList_vsdlss(longlong h);
int      removeFromMarkList_vsdlss(unsigned int *m);
longlong newCountingMark_vsdlss(int n);
void     initCountingMark_vsdlss(int n, undefined4 *m);
void     markCountingMark_vsdlss(int *m, int i);
void     unmarkCountingMark_vsdlss(int *m);

/* ---- hash table (open addressing) ---- */
longlong newHashTable_vsdlss(undefined4 sz);
longlong deleteHashTable_vsdlss(longlong h);
void     insertIntoHt_vsdlss(int key, undefined4 val, int *ht);
void     deleteBucketFromHt_vsdlss(int key, int *ht);
int      hashBucketEmpty_vsdlss(int key, int *ht);
undefined4 *newArrBucketArr_vsdlss(int n);
longlong deleteArrBucketArr_vsdlss(undefined4 *arr);
int      bucketNodesByDegree_vsdlss(int *perm, undefined8 a);   /* dummy */
void     bucketNeighborsIntoNodeBuckets_vsdlss(int *perm);
void     collectArrayBasedBuckets_vsdlss(longlong a, longlong b);
undefined4 *arrayBasedBucketByDigit_vsdlss(longlong a, int radix, int digits);

/* ---- adjacency graph container ---- */
void     newGraph_vsdlss(undefined4 n);
longlong deleteGraph_vsdlss(longlong h);
void     writeGraph_vsdlss(int *g);
longlong readGraph_vsdlss(void);
void     permuteGraph_vsdlss(long long *g, longlong perm);
longlong getGraphFromElemArr_vsdlss(undefined4 a, int n, longlong b);
void     getGraphArrFromElemArr_vsdlss(int a, int b, longlong c, longlong d);
void     convertToNullGraph_vsdlss(int *g);
void     convertG2GLT_vsdlss(int *g);
void     convertG2GUT_vsdlss(int *g);
void     convertGLT2GUT_vsdlss(int *g);
void     convertGUT2GLT_vsdlss(int *g);
void     convertGUT2G_vsdlss(int *g);
void     eliminateDuplicateEdgesInGraph_vsdlss(int *g);
void     markNodesInList_vsdlss(int n, longlong h, longlong pos);
void     renameNodesInAdjListOfGraph_vsdlss(int *g, longlong map);
longlong convertChildToParentToGraph_vsdlss(int *g, int n);
void     compressOutMarkedVars_vsdlss(longlong h, int marked, longlong newlist);
void     setCompGraphPointers_vsdlss(longlong a, longlong b, int c, int d, int e, undefined4 *f, int *g2);

/* ---- multilevel nested-dissection ordering (MLDOrder family) ---- */
void     MLDOrder_vsdlss(int n, undefined4 a, undefined8 b, undefined8 c, undefined4 d, undefined8 e, undefined8 f, undefined4 *perm, longlong opts);
void     MLDOrderInternal_vsdlss(longlong a, longlong b, undefined8 c, int *perm, undefined4 d, undefined4 e);
void     MLDCoarsen_vsdlss(int n, undefined4 a, undefined4 b, longlong c, longlong d);
void     MLDFindPartition_vsdlss(undefined8 a, undefined4 b, longlong c, longlong d, undefined8 e);
void     MLDBalance_vsdlss(longlong a, longlong b, undefined8 c);
void     MLDMinCover_vsdlss(longlong a, longlong b, undefined8 c);
void     MLDRMMatching_vsdlss(int n, longlong a, longlong b, longlong c);
void     MLDSHEMMatching_vsdlss(int n, longlong a, longlong b, undefined8 c, longlong d);
void     MLDInitPartition_vsdlss(undefined8 a, longlong b, longlong c, longlong d);
void     MLDUncoarsenRefine_vsdlss(undefined8 a, int b, undefined8 c, undefined8 d);
void     MLDUncoarsenRefineOneLevel_vsdlss(undefined8 a, int b, int c, longlong d, undefined8 e);
void     MLDInitRand_vsdlss(uint seed);
void     MLDRandomPermuteVector_vsdlss(undefined8 a, int n);
void     minDegreeOrderArr_vsdlss(int n, undefined4 b, undefined8 c, undefined8 d, int *perm, undefined8 f, longlong g);
void     MLDFinishReordering_vsdlss(int n, int m, longlong a, int *perm);
void     MLDcheckForProperReordering_vsdlss(int n, int m, longlong a);
void     orderHighDegNodesAtEnd_vsdlss(longlong a, int n, longlong b, int *perm, longlong c);
void     MMDFromMLD_vsdlss(longlong a, undefined4 b, undefined8 c, undefined8 d, undefined4 *perm);
void     MMDFromMLDStub_vsdlss(longlong a, int b, undefined8 c, longlong d, undefined4 *perm);

#endif
