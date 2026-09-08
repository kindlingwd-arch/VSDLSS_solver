/* Manual struct/pointer backfill for the remaining VSDLSS functions.
 *
 * The auto-generated vsdlss_globals.h declares every non-code global as an
 * 8-byte slot (undefined8).  Some functions dereference those globals as
 * typed pointers, which fails to compile.  Declare them here with the correct
 * base type (matching the casts the decompiled code applies).
 *
 * To enable: after regenerating vsdlss_globals.h, add the names below to the
 * EXCLUDE list in gen_globals.py (so they are not emitted as undefined8), then
 * #include "vsdlss_globals.h" *before* this header in vsdlss_ref.h.
 */
#ifndef VSDLSS_STRUCTS_H
#define VSDLSS_STRUCTS_H

#include "vsdlss_ref.h"

#if 1   /* base types picked from the cast sites in the decompiled code */

/* initChunkWrite_vsdlss : chunked-file writer state */
extern undefined4 *cmuFromNodeVecWrite;
extern undefined4 *cmuToNodeVecWrite;
extern undefined8 *cmuOffDiagVecWrite;
extern FILE        *cmuChunkedWriteFp;
extern int          cmuChunkSizeWrite;
extern int          cmuCurrentEntryWrite;

#endif
#endif
