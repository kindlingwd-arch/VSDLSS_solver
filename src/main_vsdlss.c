/* main_vsdlss.c - run the reproduced VSDLSS solver via the file entry.
 * usage: ./vsdlss_solver <jobname>   (reads <job>.hdr + mat files, writes <job>.rsl) */
#include "vsdlss_catalog.h"
int main(int argc, char **argv){ return solverVsdlss(argc, argv); }
