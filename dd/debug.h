#include "bits/stdc++.h"
// #if defined(__cpp_lib_print) && defined(__cpp_lib_format_ranges)
// #include "debug23.h"
// #else
#include "debug20.h"
// #endif

#ifdef __cpp_lib_print
inline void line_info(int num, const std::string& v) { std::print(std::cerr, "Line {}: [{}] - ", num, v); }
#else
void line_info(int num, const std::string& v) { std::cerr << "Line " << num << ": [" << v << "] - "; }
#endif

#define dbg(...) (line_info(__LINE__, #__VA_ARGS__), dbg1(__VA_ARGS__))
// #define dbgR(...) lineInfo(__LINE__, #__VA_ARGS__), dbg2(__VA_ARGS__)
#define dbg_proj(p, ...) (line_info(__LINE__, #__VA_ARGS__ " proj"), dbg3(p, __VA_ARGS__))
#define dbg_rproj(p, ...) (line_info(__LINE__, #__VA_ARGS__ " proj"), dbg4(p, __VA_ARGS__))

inline void nline() { std::cerr << '\n' << std::flush; }

inline void bar() {
  nline();
  std::cerr << "--------------------------------------------------------\n" << std::flush;
  nline();
}

[[maybe_unused]] static auto time_start = std::chrono::high_resolution_clock::now();
[[maybe_unused]] static uint_least32_t line_start = 0;

#ifdef __cpp_lib_source_location
inline void start_clock(const std::source_location loc = std::source_location::current()) {
  time_start = std::chrono::high_resolution_clock::now();
  line_start = loc.line();
}
#else
#define start_clock() (time_start = std::chrono::high_resolution_clock::now(), line_start = __LINE__)
#endif

#ifdef __cpp_lib_source_location
#ifdef __cpp_lib_print
inline void end_clock(const std::source_location loc = std::source_location::current()) {
  std::println(
      std::cerr, "Lines {}-{}: {}", line_start, loc.line(),
      std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - time_start));
}
#else
void end_clock(const std::source_location loc = std::source_location::current()) {
  std::cerr << std::format(
      "Lines {}-{}: {}\n", line_start, loc.line(),
      std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - time_start));
}
#endif
#else
#define end_clock()                                                                                                    \
  (std::cerr                                                                                                           \
   << "Lines "                                                                                                         \
   << line_start                                                                                                       \
   << '-'                                                                                                              \
   << __LINE__                                                                                                         \
   << ": "                                                                                                             \
   << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - time_start)    \
   << '\n')
#endif
