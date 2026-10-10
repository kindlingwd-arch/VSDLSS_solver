// =============================================================================
//  examples/transient_driver.cpp —— 一次分解、多步求解（瞬态）的驱动示例
//
//  读入文本矩阵与右端项，用公开接口 vsdlss_factorize_m3（METIS，ordering 6）
//  分解一次，再对第 1..N 步的右端项反复调用 vsdlss_m3_solve，并与参考解比较。
//  VsdlssSolver 的 setup_vsdlss / run_vsdlss / gett_rhs 与 SolveCase 语义沿用
//  原 CASI 驱动：FIRST = 分解 + 求解；SUBSEQUENT = 复用因子只求解；
//  SUBSEQUENT_A_CHANGED / REFACTOR_NONLINEAR = 换数值后重新分解 + 求解；CLEANUP = 释放。
//
//  文件（当前目录）：diag.txt "i value"（或每行一个 value），data.txt
//  "row col value"（严格上三角、0 基、不重复），b_vector.txt / x_vector.txt
//  "i value"，以及第 s 步的 <s>b_vector.txt / <s>x_vector.txt。
//
//  大规模（几千万结点）时要注意的几点，本示例都已处理：
//   1. 内存：文本逐行流式解析（不把整个文件读成字符串数组）；去重用排序
//      （std::set 在 6400 万规模要数 GB，释放后仍留在堆里）；建完 CSC 即释放
//      中间矩阵，残差用 CSC 计算；不复制 b（vsdlss_m3_solve 不改写 b）；参考解
//      逐行比较，不整体载入；装载后 malloc_trim 归还空闲内存。调用方内存加上
//      因子超过物理内存时，换页会让求解慢数倍。
//   2. 环境：默认 MKL_THREADING_LAYER=SEQUENTIAL（求解器内 BLAS 必须单线程）、
//      VSDLSS_BLAS_MIN=16；不强制 VSDLSS_BLAS_SOLVE_MIN（库默认值：宽面板走
//      BLAS 且保留树并行）。线程数：VSDLSS_THREADS=n 或 auto；未设时用库的
//      VSDLSS_NUM_THREADS；都没有时自动（OMP_NUM_THREADS / 进程可用的 CPU）。
//   3. 计时与诊断：分别报告分解、第一次求解（冷）、后续各步求解（热）及其
//      中位数；每次求解的缺页次数（major fault > 0 说明在换页）与峰值内存；
//      VSDLSS_TRACE=1 时库在 stderr 打印每次求解的分阶段耗时。
//   4. STEPS=N：最多 N 步（默认 40），遇到缺失的 <s>b_vector.txt 即停止。
//      CHECK_RESIDUAL=0：不算残差，分解后释放 CSC（再省 nnz*16 字节）。
//
//  构建与运行（见 docs/user/QUICKSTART.md 第 7 节）
//    make METIS=1 BLAS=1 BLAS_LIBS='-L<mkl>/lib -lmkl_rt -Wl,-rpath,<mkl>/lib' transient_driver
//    VSDLSS_THREADS=32 OMP_PROC_BIND=spread OMP_PLACES=cores ./transient_driver diag.txt data.txt
// =============================================================================
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cerrno>
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <locale>
#include <numeric>
#include <sstream>
#include <stdexcept>
#include <string>

#include <utility>
#include <vector>
#include <sys/resource.h>
#ifdef __GLIBC__
#include <malloc.h>
static void releaseFreeMemory() { malloc_trim(0); }
#else
static void releaseFreeMemory() {}
#endif

extern "C" {
#include "vsdlss.h"
}

enum SolveCase { SINGLE = 0, FIRST = 1, SUBSEQUENT = 2, SUBSEQUENT_A_CHANGED = 3, REFACTOR_NONLINEAR, CLEANUP = -1 };

struct Entry { std::int64_t row, col; double value; };

static std::string at(const std::string& path, std::size_t line) { return path + ":" + std::to_string(line); }
static double finite(double x, const std::string& where) {
    if (!std::isfinite(x)) throw std::runtime_error("Non-finite value at " + where);
    return x;
}

// Calls f(line_number, line) for every non-blank, non-comment line (streaming).
template <class F> static void forRecords(const std::string& path, F f) {
    std::ifstream in(path);
    if (!in) throw std::runtime_error("Cannot open " + path);
    std::string line;
    for (std::size_t number = 1; std::getline(in, line); ++number) {
        auto p = line.find_first_not_of(" \t\r");
        if (p != std::string::npos && line[p] != '#') f(number, line);
    }
    if (in.bad()) throw std::runtime_error("Error reading " + path);
}

// strtod / strtoll parsing (locale "C" numbers, as the original's classic locale).
static bool parseI64(const char*& s, std::int64_t& v) { char* e; errno = 0; long long x = std::strtoll(s, &e, 10); if (e == s || errno) return false; v = x; s = e; return true; }
static bool parseF64(const char*& s, double& v) { char* e; double x = std::strtod(s, &e); if (e == s) return false; v = x; s = e; return true; }
static bool restBlank(const char* s) { while (*s == ' ' || *s == '\t' || *s == '\r') ++s; return *s == 0; }

struct Matrix {
    std::vector<double> diagonal;
    std::vector<Entry> upper;   // strict upper triangle, zero-based, each pair once

    static Matrix fromFiles(const std::string& diagPath, const std::string& dataPath) {
        Matrix m;
        forRecords(diagPath, [&](std::size_t no, const std::string& line) {
            const char* s = line.c_str(); double a, b; std::int64_t row;
            const char* t = s;
            if (parseF64(t, a) && restBlank(t)) { m.diagonal.push_back(finite(a, at(diagPath, no))); return; }
            t = s;
            if (!parseI64(t, row) || row != (std::int64_t)m.diagonal.size() || !parseF64(t, b) || !restBlank(t))
                throw std::runtime_error("Invalid diagonal at " + at(diagPath, no));
            m.diagonal.push_back(finite(b, at(diagPath, no)));
        });
        if (m.diagonal.empty()) throw std::runtime_error("Empty diagonal: " + diagPath);
        const std::int64_t n = (std::int64_t)m.diagonal.size();
        forRecords(dataPath, [&](std::size_t no, const std::string& line) {
            const char* t = line.c_str(); std::int64_t r, c; double v;
            if (!parseI64(t, r) || !parseI64(t, c) || !parseF64(t, v) || !restBlank(t))
                throw std::runtime_error("Expected 'row col value' at " + at(dataPath, no));
            if (r < 0 || c < 0 || r >= n || c >= n) throw std::runtime_error("Index out of range at " + at(dataPath, no));
            if (r >= c) throw std::runtime_error("Expected strict upper entry at " + at(dataPath, no));
            m.upper.push_back({r, c, finite(v, at(dataPath, no))});
        });
        // Duplicate check by sorting an index array (the original's std::set takes
        // several GB at 64M and its freed nodes stay in the heap).
        std::vector<std::uint64_t> key(m.upper.size());
        for (std::size_t k = 0; k < key.size(); ++k) key[k] = (std::uint64_t)m.upper[k].row * (std::uint64_t)n + (std::uint64_t)m.upper[k].col;
        std::sort(key.begin(), key.end());
        if (std::adjacent_find(key.begin(), key.end()) != key.end()) throw std::runtime_error("Duplicate upper entry in " + dataPath);
        return m;
    }
};

static std::vector<double> loadVector(const std::string& path, std::size_t n) {
    std::vector<double> v; v.reserve(n);
    forRecords(path, [&](std::size_t no, const std::string& line) {
        const char* t = line.c_str(); std::int64_t i; double x;
        if (!parseI64(t, i) || i != (std::int64_t)v.size() || !parseF64(t, x) || !restBlank(t))
            throw std::runtime_error("Expected sequential 'index value' at " + at(path, no));
        v.push_back(finite(x, at(path, no)));
    });
    if (v.size() != n) throw std::runtime_error("Vector length differs from matrix: " + path);
    return v;
}

// max |x - ref| and max |ref| with ref read line by line (no n-vector in memory).
static std::pair<double, double> compareWithFile(const std::string& path, const double* x, std::size_t n) {
    double err = 0, norm = 0; std::size_t k = 0;
    forRecords(path, [&](std::size_t no, const std::string& line) {
        const char* t = line.c_str(); std::int64_t i; double r;
        if (!parseI64(t, i) || i != (std::int64_t)k || k >= n || !parseF64(t, r) || !restBlank(t))
            throw std::runtime_error("Expected sequential 'index value' at " + at(path, no));
        err = std::max(err, std::abs(x[k] - r)); norm = std::max(norm, std::abs(r)); ++k;
    });
    if (k != n) throw std::runtime_error("Vector length differs from matrix: " + path);
    return {err, norm};
}

// ||b - A x||_inf / max(1, ||b||_inf) from the upper CSC (diagonal last in each column).
static double relativeResidual(const vsdlss* A, const double* b, const double* x) {
    const csi n = A->n;
    std::vector<double> ax(n, 0.0);
    for (csi j = 0; j < n; ++j)
        for (csi p = A->p[j]; p < A->p[j + 1]; ++p) {
            const csi i = A->i[p]; const double v = A->x[p];
            ax[j] += v * x[i];
            if (i != j) ax[i] += v * x[j];
        }
    double r = 0, bn = 0;
    for (csi i = 0; i < n; ++i) { r = std::max(r, std::abs(b[i] - ax[i])); bn = std::max(bn, std::abs(b[i])); }
    return r / std::max(1.0, bn);
}

struct Usage { double maxrss_gb; long majflt, minflt; };
static Usage usage() { rusage u; getrusage(RUSAGE_SELF, &u); return {u.ru_maxrss / 1048576.0, u.ru_majflt, u.ru_minflt}; }

class VsdlssSolver {
public:
    // threads: n, 0 = automatic (OMP_NUM_THREADS / CPU affinity), < 0 = the
    // library default (VSDLSS_NUM_THREADS, else 1).  Per calling thread: set it
    // on the thread that factors and solves.
    explicit VsdlssSolver(int threads, int order = 6) : threads_(threads), order_(order) {
        if (threads_ >= 0) check(vsdlss_set_num_threads(threads_), "vsdlss_set_num_threads");
    }
    ~VsdlssSolver() { release(); }
    VsdlssSolver(const VsdlssSolver&) = delete;
    VsdlssSolver& operator=(const VsdlssSolver&) = delete;

    // keepPattern: keep the entry -> CSC position map for update_values (8 bytes per off-diagonal).
    void setup_vsdlss(const Matrix& A, bool keepPattern = true) {
        release();
        n_ = A.diagonal.size(); offCount_ = A.upper.size();
        A_ = vsdlss_spalloc((csi)n_, (csi)n_, (csi)(n_ + offCount_), 1, 0);
        if (!A_) throw std::runtime_error("vsdlss_spalloc: out of memory");
        std::vector<csi> cursor(n_ + 1, 0);
        for (const auto& e : A.upper) ++cursor[(std::size_t)e.col + 1];
        A_->p[0] = 0;
        for (std::size_t j = 0; j < n_; ++j) A_->p[j + 1] = A_->p[j] + cursor[j + 1] + 1;
        // rows ascending inside each column: visit entries by row (counting sort)
        std::vector<std::size_t> rowPtr(n_ + 1, 0), byRow(offCount_);
        for (const auto& e : A.upper) ++rowPtr[(std::size_t)e.row + 1];
        for (std::size_t i = 0; i < n_; ++i) rowPtr[i + 1] += rowPtr[i];
        for (std::size_t k = 0; k < offCount_; ++k) byRow[rowPtr[(std::size_t)A.upper[k].row]++] = k;
        std::vector<std::size_t>().swap(rowPtr);
        for (std::size_t j = 0; j < n_; ++j) cursor[j] = A_->p[j];
        if (keepPattern) upperPos_.resize(offCount_);
        for (std::size_t k : byRow) {
            const auto& e = A.upper[k];
            const csi pos = cursor[(std::size_t)e.col]++;
            A_->i[pos] = e.row; A_->x[pos] = e.value;
            if (keepPattern) upperPos_[k] = pos;
        }
        for (std::size_t j = 0; j < n_; ++j) { A_->i[A_->p[j + 1] - 1] = (csi)j; A_->x[A_->p[j + 1] - 1] = A.diagonal[j]; }
        check(vsdlss_validate_upper_csc(A_), "vsdlss_validate_upper_csc");
        x_.assign(n_, 0.0);
    }

    void update_values(const Matrix& A) {
        if (!A_ || upperPos_.size() != offCount_ || A.diagonal.size() != n_ || A.upper.size() != offCount_)
            throw std::runtime_error("update_values: needs setup_vsdlss(A, true) and the same pattern");
        for (std::size_t k = 0; k < offCount_; ++k) {
            if (A_->i[upperPos_[k]] != A.upper[k].row) throw std::runtime_error("update_values: pattern differs");
            A_->x[upperPos_[k]] = A.upper[k].value;
        }
        for (std::size_t j = 0; j < n_; ++j) A_->x[A_->p[j + 1] - 1] = A.diagonal[j];
    }

    void run_vsdlss(int mode, const double* bx) {
        switch (mode) {
        case SINGLE: case FIRST: case SUBSEQUENT_A_CHANGED: case REFACTOR_NONLINEAR: factor(); solve(bx); break;
        case SUBSEQUENT: if (!f_) throw std::runtime_error("SUBSEQUENT before FIRST"); solve(bx); break;
        case CLEANUP: release(); break;
        default: throw std::runtime_error("Unknown SolveCase " + std::to_string(mode));
        }
    }
    double* gett_rhs() { return x_.data(); }
    const vsdlss* matrix() const { return A_; }
    void drop_matrix() { if (A_) { vsdlss_spfree(A_); A_ = nullptr; } std::vector<csi>().swap(upperPos_); }

    double factor_ms = 0, solve_only_ms = 0;
    int observedThreads() const { return vsdlss_parallel_last_team_size(); }

private:
    using clock = std::chrono::steady_clock;
    static double msSince(clock::time_point t) { return std::chrono::duration<double, std::milli>(clock::now() - t).count(); }
    static void check(vsdlss_status st, const char* where) {
        if (st != VSDLSS_OK) throw std::runtime_error(std::string(where) + ": " + vsdlss_status_string(st));
    }
    void factor() {
        if (!A_) throw std::runtime_error("factor without a matrix");
        if (f_) { vsdlss_m3_factor_free(f_); f_ = nullptr; }
        const auto t = clock::now();
        const vsdlss_status st = vsdlss_factorize_m3(A_, order_, &f_);
        factor_ms = msSince(t);
        if (st == VSDLSS_ERR_UNSUPPORTED && order_ == 6)
            throw std::runtime_error("ordering 6 (METIS) unsupported: build libvsdlss.a with make METIS=1");
        check(st, "vsdlss_factorize_m3");
    }
    void solve(const double* b) {
        const auto t = clock::now();
        check(vsdlss_m3_solve(f_, b, x_.data()), "vsdlss_m3_solve");
        solve_only_ms = msSince(t);
    }
    void release() {
        if (f_) { vsdlss_m3_factor_free(f_); f_ = nullptr; }
        drop_matrix();
    }
    int threads_, order_;
    vsdlss* A_ = nullptr;
    vsdlss_m3_factor* f_ = nullptr;
    std::size_t n_ = 0, offCount_ = 0;
    std::vector<csi> upperPos_;
    std::vector<double> x_;
};

static void setDefaultEnv(const char* name, const char* value) { setenv(name, value, 0); }
static int envInt(const char* name, int def) { const char* e = std::getenv(name); return e && *e ? std::atoi(e) : def; }

int main(int argc, char** argv) {
    try {
        // Before the first call into the library / MKL.
        setDefaultEnv("MKL_THREADING_LAYER", "SEQUENTIAL");
        setDefaultEnv("VSDLSS_BLAS_MIN", "16");
        // VSDLSS_THREADS = n or auto; else VSDLSS_NUM_THREADS (read by the
        // library) if set; else automatic.
        int threads = 0;
        if (const char* e = std::getenv("VSDLSS_THREADS"); e && *e) threads = std::max(0, std::strcmp(e, "auto") ? std::atoi(e) : 0);
        else if (std::getenv("VSDLSS_NUM_THREADS")) threads = -1;
        const int maxSteps = envInt("STEPS", 40);
        const bool residual = envInt("CHECK_RESIDUAL", 1) != 0;
        const std::string diagPath = argc > 1 ? argv[1] : "diag.txt";
        const std::string dataPath = argc > 2 ? argv[2] : "data.txt";
        const auto now = [] { return std::chrono::steady_clock::now(); };
        const auto ms = [](auto a, auto b) { return std::chrono::duration<double, std::milli>(b - a).count(); };

        VsdlssSolver solver(threads, 6);
        std::size_t n = 0, off = 0;
        auto t0 = now();
        {
            const Matrix a = Matrix::fromFiles(diagPath, dataPath);
            n = a.diagonal.size(); off = a.upper.size();
            solver.setup_vsdlss(a, /*keepPattern=*/false);
        }   // the Matrix copy is freed here: the CSC is the only copy of A
        releaseFreeMemory();
        auto t1 = now();
        std::vector<double> b = loadVector("b_vector.txt", n);
        const Usage u0 = usage();
        auto t2 = now();
        solver.run_vsdlss(FIRST, b.data());
        auto t3 = now();
        const Usage u1 = usage();
        double res = -1;
        if (residual) res = relativeResidual(solver.matrix(), b.data(), solver.gett_rhs());
        else { solver.drop_matrix(); releaseFreeMemory(); }
        const auto [err, norm] = compareWithFile("x_vector.txt", solver.gett_rhs(), n);
        std::cout << std::setprecision(6)
                  << "n=" << n << " off_diagonal=" << off << '\n'
                  << "solver=libvsdlss M3 order=6 threads=" << vsdlss_get_num_threads() << " max_team=" << solver.observedThreads() << '\n'
                  << "env MKL_THREADING_LAYER=" << std::getenv("MKL_THREADING_LAYER")
                  << " VSDLSS_BLAS_SOLVE_MIN=" << (std::getenv("VSDLSS_BLAS_SOLVE_MIN") ? std::getenv("VSDLSS_BLAS_SOLVE_MIN") : "(library default)") << '\n'
                  << "load+setup_ms=" << ms(t0, t1) << '\n'
                  << "first: factor_ms=" << solver.factor_ms << " solve_ms(cold)=" << solver.solve_only_ms
                  << " total_ms=" << ms(t2, t3) << '\n'
                  << "  peak_rss_gb=" << u1.maxrss_gb << " major_faults_during_factor+solve=" << (u1.majflt - u0.majflt) << '\n'
                  << "relative_residual_inf=" << res << " relative_error_vs_x_inf=" << err / std::max(1.0, norm) << "\n\n";

        std::vector<double> warm;
        for (int step = 1; step <= maxSteps; ++step) {
            const std::string bf = std::to_string(step) + "b_vector.txt", xf = std::to_string(step) + "x_vector.txt";
            if (!std::ifstream(bf)) break;
            auto l0 = now();
            b = loadVector(bf, n);                                   // reuse b's storage
            auto l1 = now();
            const Usage s0 = usage();
            solver.run_vsdlss(SUBSEQUENT, b.data());
            const Usage s1 = usage();
            warm.push_back(solver.solve_only_ms);
            const double r = residual ? relativeResidual(solver.matrix(), b.data(), solver.gett_rhs()) : -1;
            const auto [e, nn] = compareWithFile(xf, solver.gett_rhs(), n);
            std::cout << "step " << step << ": load_ms=" << ms(l0, l1) << " solve_ms=" << solver.solve_only_ms
                      << " major_faults=" << (s1.majflt - s0.majflt) << " minor_faults=" << (s1.minflt - s0.minflt)
                      << " residual=" << r << " rel_err=" << e / std::max(1.0, nn) << '\n';
        }
        if (!warm.empty()) {
            std::vector<double> s = warm; std::sort(s.begin(), s.end());
            std::cout << "\nwarm solves: " << s.size() << ", median " << s[s.size() / 2] << " ms, min " << s.front()
                      << " ms, max " << s.back() << " ms | peak_rss_gb=" << usage().maxrss_gb << '\n';
        }
        solver.run_vsdlss(CLEANUP, nullptr);
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << '\n';
        return 1;
    }
}
