.text
.globl cgemm_
.type cgemm_,@function
cgemm_: jmp scipy_cgemm_@PLT
.globl cgemv_
.type cgemv_,@function
cgemv_: jmp scipy_cgemv_@PLT
.globl cherk_
.type cherk_,@function
cherk_: jmp scipy_cherk_@PLT
.globl cpotrf_
.type cpotrf_,@function
cpotrf_: jmp scipy_cpotrf_@PLT
.globl ctrsm_
.type ctrsm_,@function
ctrsm_: jmp scipy_ctrsm_@PLT
.globl ctrsv_
.type ctrsv_,@function
ctrsv_: jmp scipy_ctrsv_@PLT
.globl dgemm_
.type dgemm_,@function
dgemm_: jmp scipy_dgemm_@PLT
.globl dgemv_
.type dgemv_,@function
dgemv_: jmp scipy_dgemv_@PLT
.globl dpotrf_
.type dpotrf_,@function
dpotrf_: jmp scipy_dpotrf_@PLT
.globl dsyrk_
.type dsyrk_,@function
dsyrk_: jmp scipy_dsyrk_@PLT
.globl dtrsm_
.type dtrsm_,@function
dtrsm_: jmp scipy_dtrsm_@PLT
.globl dtrsv_
.type dtrsv_,@function
dtrsv_: jmp scipy_dtrsv_@PLT
.globl sgemm_
.type sgemm_,@function
sgemm_: jmp scipy_sgemm_@PLT
.globl sgemv_
.type sgemv_,@function
sgemv_: jmp scipy_sgemv_@PLT
.globl spotrf_
.type spotrf_,@function
spotrf_: jmp scipy_spotrf_@PLT
.globl ssyrk_
.type ssyrk_,@function
ssyrk_: jmp scipy_ssyrk_@PLT
.globl strsm_
.type strsm_,@function
strsm_: jmp scipy_strsm_@PLT
.globl strsv_
.type strsv_,@function
strsv_: jmp scipy_strsv_@PLT
.globl zgemm_
.type zgemm_,@function
zgemm_: jmp scipy_zgemm_@PLT
.globl zgemv_
.type zgemv_,@function
zgemv_: jmp scipy_zgemv_@PLT
.globl zherk_
.type zherk_,@function
zherk_: jmp scipy_zherk_@PLT
.globl zpotrf_
.type zpotrf_,@function
zpotrf_: jmp scipy_zpotrf_@PLT
.globl ztrsm_
.type ztrsm_,@function
ztrsm_: jmp scipy_ztrsm_@PLT
.globl ztrsv_
.type ztrsv_,@function
ztrsv_: jmp scipy_ztrsv_@PLT
.section .note.GNU-stack,"",@progbits
