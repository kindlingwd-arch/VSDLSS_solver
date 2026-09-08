/* harness: MapPartitioned3d addr=005f9210 size=2ee */
#include "vsdlss_ref.h"


/* MapPartitioned3d<double, int>::MapPartitioned3d(MatrixPartition<int>&, std::__cxx11::string
   const&, bool) */

void __thiscall
MapPartitioned3d<double,int>::MapPartitioned3d
          (MapPartitioned3d<double,int> *this,MatrixPartition *param_1,string *param_2,bool param_3)

{
  int iVar1;
  undefined1 *__src;
  ulong __n;
  int iVar2;
  ulong local_60;
  ulong *local_58;
  ulong local_50;
  ulong local_48 [3];
  
  *(MatrixPartition **)this = param_1;
  Symmetric<double,int>::Symmetric((Symmetric<double,int> *)(this + 0x18),param_2,param_3);
  local_50 = 3;
  local_48[0] = CONCAT44(local_48[0]._4_4_,0x323161);
  local_58 = local_48;
                    /* try { // try from 005f9273 to 005f9277 has its CatchHandler @ 005f9573 */
  General<double,int,false>::General
            ((General<double,int,false> *)(this + 0x58),param_2,(string *)&local_58,param_3);
  if (local_58 != local_48) {
    tc_cfree(local_58);
  }
  local_50 = 3;
  local_48[0] = CONCAT44(local_48[0]._4_4_,0x323261);
  local_58 = local_48;
                    /* try { // try from 005f92c4 to 005f92c8 has its CatchHandler @ 005f9522 */
  General<double,int,true>::General
            ((General<double,int,true> *)(this + 0x90),param_2,(string *)&local_58,param_3);
  if (local_58 != local_48) {
    tc_cfree(local_58);
  }
  local_50 = 3;
  local_48[0] = CONCAT44(local_48[0]._4_4_,0x333161);
  local_58 = local_48;
                    /* try { // try from 005f9315 to 005f9319 has its CatchHandler @ 005f9590 */
  General<double,int,false>::General
            ((General<double,int,false> *)(this + 200),param_2,(string *)&local_58,param_3);
  if (local_58 != local_48) {
    tc_cfree(local_58);
  }
  __src = *(undefined1 **)param_2;
  __n = *(ulong *)(param_2 + 8);
  if ((__src + __n != (undefined1 *)0x0) && (__src == (undefined1 *)0x0)) {
    local_58 = local_48;
                    /* WARNING: Subroutine does not return */
                    /* try { // try from 005f9501 to 005f9505 has its CatchHandler @ 005f9518 */
    std::__throw_logic_error("basic_string::_M_construct null not valid");
  }
  local_60 = __n;
  if (__n < 0x10) {
    if (__n == 1) {
      local_48[0] = CONCAT71(local_48[0]._1_7_,*__src);
      local_58 = local_48;
      goto LAB_005f9381;
    }
    local_58 = local_48;
    if (__n == 0) goto LAB_005f9381;
  }
  else {
    local_58 = local_48;
                    /* try { // try from 005f947c to 005f9480 has its CatchHandler @ 005f9518 */
    local_58 = (ulong *)std::__cxx11::string::_M_create((ulong *)&local_58,(ulong)&local_60);
    local_48[0] = local_60;
  }
  memcpy(local_58,__src,__n);
LAB_005f9381:
  *(undefined1 *)((long)local_58 + local_60) = 0;
  local_50 = local_60;
  if (0x3fffffffffffffff - local_60 < 4) {
                    /* WARNING: Subroutine does not return */
                    /* try { // try from 005f950b to 005f950f has its CatchHandler @ 005f9595 */
    std::__throw_length_error("basic_string::append");
  }
                    /* try { // try from 005f93b3 to 005f93b7 has its CatchHandler @ 005f9595 */
  std::__cxx11::string::_M_append((char *)&local_58,0xabca15);
                    /* try { // try from 005f93c5 to 005f93c9 has its CatchHandler @ 005f951d */
  Symmetric<double,int>::Symmetric
            ((Symmetric<double,int> *)(this + 0x100),(string *)&local_58,param_3);
  if (local_58 != local_48) {
    tc_cfree(local_58);
  }
  iVar2 = *(int *)(this + 0x90);
  iVar1 = *(int *)(this + 0x18);
  *(undefined8 *)(this + 0x140) = 0;
  *(undefined8 *)(this + 0x148) = 0;
  *(int *)(this + 8) = iVar1;
  this[0x14] = (MapPartitioned3d<double,int>)(iVar2 == 0);
  *(int *)(this + 0xc) = iVar2;
  *(int *)(this + 0x10) = *(int *)(this + 0x100);
  this[0x15] = (MapPartitioned3d<double,int>)(*(int *)(this + 0x100) == 0);
  if (iVar1 < *(int *)(this + 0x58)) {
    asim_error("A12 rows > A11 rows");
    iVar2 = *(int *)(this + 0xc);
  }
  else {
    *(int *)(this + 0x58) = iVar1;
  }
  if (iVar2 < *(int *)(this + 0x5c)) {
    asim_error("A12 cols > A22 cols");
    iVar2 = *(int *)(this + 0xc);
  }
  else {
    *(int *)(this + 0x5c) = iVar2;
  }
  if (iVar2 < *(int *)(this + 0x94)) {
                    /* try { // try from 005f94b7 to 005f94f3 has its CatchHandler @ 005f956e */
    asim_error("A22 cols > A22 rows");
    return;
  }
  *(int *)(this + 0x94) = iVar2;
  return;
}


