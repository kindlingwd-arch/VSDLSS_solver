#!/usr/bin/env python3
"""Generate a diagnostic-only panel translation unit; production source unchanged."""
from pathlib import Path
import sys
src=Path(sys.argv[1]).read_text()
head='''#define vsdlss_panel_factor profile_panel_factor
#define vsdlss_panel_solve_generic profile_panel_solve_generic
#define vsdlss_panel_solve profile_panel_solve
extern double solve_profile_clock(void);
extern void solve_profile_diag_add(long long width, double elapsed);
'''
# Both production backward diagonal loops (generic and SIMD narrow).
a='    if(back)for(csi j=width;j-- >0;){'
assert src.count(a)==1
src=src.replace(a,'    double profile_t0 = back && width>=64 ? solve_profile_clock() : 0;\n'+a)
start=src.index('    double profile_t0')
end=src.index('    return VSDLSS_OK;',start)
src=src[:end]+'    if(back && width>=64)solve_profile_diag_add(width,solve_profile_clock()-profile_t0);\n'+src[end:]
a='    for(csi j=width;j-- >0;){'
assert src.count(a)==1
src=src.replace(a,'    double profile_t0 = width>=64 ? solve_profile_clock() : 0;\n'+a)
start=src.index('    double profile_t0 = width>=64')
end=src.index('    return VSDLSS_OK;',start)
src=src[:end]+'    if(width>=64)solve_profile_diag_add(width,solve_profile_clock()-profile_t0);\n'+src[end:]
Path(sys.argv[2]).write_text(head+src)
