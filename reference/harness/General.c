/* harness: General addr=005f91b0 size=45 */
#include "vsdlss_ref.h"


/* General<double, int, false>::General(std::__cxx11::string const&, std::__cxx11::string const&,
   bool) */

void __thiscall
General<double,int,false>::General
          (General<double,int,false> *this,string *param_1,string *param_2,bool param_3)

{
  *(undefined8 *)(this + 0x10) = 0;
  *(undefined8 *)(this + 0x18) = 0;
  *(undefined8 *)(this + 0x20) = 0;
  *(undefined8 *)(this + 0x30) = 0;
  if (param_3) {
                    /* try { // try from 005f91dd to 005f91f4 has its CatchHandler @ 005f91fc */
    solver_read<long>(this,param_1,param_2);
    return;
  }
  solver_read<int>(this,param_1,param_2);
  return;
}


