// SPDX-License-Identifier: BSD-2-Clause OR Apache-2.0
#include <hint.h>

hint_inline hint_const constexpr int twice(int x) noexcept { return 2 * x; }
hint_noinline hint_pure int read(int const * x hint_noescape) noexcept { return *x; }
struct owner {
  int value = 3;
  int const & get() const noexcept hint_lifetimebound { return value; }
};
struct hint_lockable mutex {
  void lock() hint_exclusive_lock_function() {}
  void unlock() hint_unlock_function() {}
};

#if defined(__clang__)
template<class T> struct named;
using named_int = named<int>;
template<class T> struct hint_preferred_name(named_int) named {};
int nonnegative(int x) noexcept hint_diagnose_if(x < 0, "nonnegative argument required") { return x; }
#endif

#if defined(__x86_64__) || defined(_M_X64)
hint_target("sse2") int targeted(int x) noexcept { return x + 1; }
#if defined(__linux__)
hint_target_clones("default", "sse4.2") int cloned(int x) noexcept { return x + 1; }
#endif
#endif

static_assert(twice(3) == 6);
int main() {
  owner x;
  mutex m;
  m.lock();
  auto result = read(&x.get());
  m.unlock();
  return result != 3;
}
