# hint

Compiler annotations without a dependency on somebody else's library.

```cpp
#include <hint.h>

hint_inline hint_pure int read(int const * p hint_noescape) noexcept {
  return *p;
}
```

Hint gives compiler attributes short, consistent names for inlining, lifetimes,
ownership, thread safety and optimization. It is one header. There is nothing to
link, no runtime, and no module build. Use standard spellings such as
`[[nodiscard]]` when the language already says what you mean.

## Use it

With an installed package:

```cmake
find_package(hint CONFIG REQUIRED)
target_link_libraries(your_library PUBLIC hint::hint)
```

Or add the source tree directly:

```cmake
add_subdirectory(path/to/hint)
target_link_libraries(your_library PUBLIC hint::hint)
```

`hint::hint` is the only exported target. It supplies the include directory and
nothing else: no ISA flags, exception policy, language standard or compiler
extensions are imposed on consumers. Include `<hint.h>` wherever its macros are
used. In a C++ module, put the include in the global module fragment; imports do
not transport macros.

CMake 3.23 or newer installs the header and relocatable package configuration:

```sh
cmake -S . -B build
cmake --install build --prefix /path/to/hint
```

CMake is optional. Adding `include/` to your compiler's include path is enough.
Clang has the fullest annotation support. Other compilers receive the attributes
recognized by their feature probes; unsupported annotations generally expand to
nothing. Some visibility and debugging fallbacks warn. A hint is still a promise
to the optimizer or analyzer: lying about purity, alignment or ownership can
make otherwise ordinary code wrong.

## The catalog

See the [attribute reference](https://ekmett.github.io/hint/hint_8h.html) for declarations and documentation.

| Purpose | Annotations |
| --- | --- |
| Detection | `hint_has_attribute`, `hint_has_declspec_attribute` |
| Inlining and optimization | `hint_inline`, `hint_noinline`, `hint_flatten`, `hint_optnone`, `hint_artificial`, `hint_hot`, `hint_cold` |
| Purity | `hint_pure`, `hint_const` |
| Lifetimes | `hint_lifetimebound`, `hint_noescape`, `hint_reinitializes` |
| Preconditions | `hint_diagnose_if` |
| Linkage and instantiation | `hint_weak`, `hint_internal_linkage`, `hint_visibility`, `hint_hidden`, `hint_exclude_from_explicit_instantiation`, `hint_preferred_name` |
| Initialization and layout | `hint_constinit`, `hint_uninitialized`, `hint_empty_bases` |
| Target features | `hint_target`, `hint_target_clones` |
| Typestate | `hint_consumable`, `hint_callable_when`, `hint_param_typestate`, `hint_return_typestate`, `hint_moving`, `hint_set_typestate`, `hint_test_typestate` |
| Locks | `hint_lockable`, `hint_scoped_lockable`, `hint_guarded_by`, `hint_pt_guarded_by`, `hint_guarded_var`, `hint_pt_guarded_var`, `hint_acquired_before`, `hint_acquired_after`, `hint_exclusive_lock_function`, `hint_shared_trylock_function`, `hint_unlock_function`, `hint_lock_returned`, `hint_locks_excluded`, `hint_exclusive_locks_required`, `hint_shared_locks_required`, `hint_no_thread_safety_analysis` |
| Handles | `hint_acquire_handle`, `hint_release_handle`, `hint_use_handle` |
| Memory | `hint_align`, `hint_align_value`, `hint_assume_aligned`, `hint_malloc`, `hint_alloc_align`, `hint_alloc_size` |
| Calls and pointers | `hint_callback`, `hint_noreturn`, `hint_returns_nonnull`, `hint_nonnull`, `hint_null_terminated_string_arg`, `hint_Nonnull`, `hint_Nullable`, `hint_Null_unspecified` |
| Allocation and blocking | `hint_allocating`, `hint_nonallocating`, `hint_noalloc`, `hint_blocking`, `hint_nonblocking`, `hint_noblock` |
| Shared host/device declarations | `hint_host`, `hint_device`, `hint_hd`, `hint_global`, `hint_constexpr` |

`hint_target` selects compiler target features; it does not detect the running
CPU. `hint_target_clones` requests compiler multiversioning where supported.
Neither supplies an ISA vocabulary or a dispatch framework.

Define `HINT_USE_DECLSPEC` before including the header to opt into probed
`__declspec` attributes outside MSVC mode. Enable any compiler syntax extensions
those attributes require in your own target. The header does not enable them.
The non-C++ branch retains the small set of modifiers used in shared shader
sources; it is not a promise that every host annotation is a shader annotation.

## Tests and documentation

```sh
cmake -S . -B build -DHINT_BUILD_TESTS=ON -DHINT_BUILD_DOCS=ON
cmake --build build
ctest --test-dir build --output-on-failure
```

Documentation requires Doxygen 1.18+, Pandoc 3.8+ and Python 3. Open `build/site/index.html`.
CI builds the API documentation and publishes [the reference](https://ekmett.github.io/hint/)
to GitHub Pages on pushes to `main`.
It also compiles a consumer against a relocated installation.

Hint's catalog originated in [Native](https://github.com/ekmett/native).
The code is available under either the BSD 2-Clause or Apache 2.0 license;
see [LICENSE.md](LICENSE.md).
