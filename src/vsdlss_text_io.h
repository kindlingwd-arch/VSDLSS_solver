#ifndef VSDLSS_TEXT_IO_H
#define VSDLSS_TEXT_IO_H
#include "vsdlss.h"

/* Read zero-based text dumps from a directory; caller owns all three outputs. */
vsdlss_status vsdlss_load_text_dump(const char *directory, vsdlss **A,
                                    double **rhs, double **reference);
#endif
