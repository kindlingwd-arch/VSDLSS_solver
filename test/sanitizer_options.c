/* Test-only default for ptrace/restricted runners where LSan cannot inspect
 * /proc. ASan and UBSan remain enabled. Never linked by production targets.
 * Mirrors the existing make sanitizers ASAN_OPTIONS=detect_leaks=0 policy. */
const char *__asan_default_options(void)
{
    return "detect_leaks=0";
}
