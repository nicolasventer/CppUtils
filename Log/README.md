# Print / Log

Small headers only: include `Print.h` for type-friendly printing, or `Log.h` for leveled logs (`Log.h` includes `Print.h`). Project-wide tweaks go in **`PrintConfig.h`** (included by `Print.h` after the standard library headers) and **`LogConfig.h`** (included by `Log.h` after `DEBUG` … `FATAL` are defined).

# Print

## Configuration (`PrintConfig.h`)

`Print.h` includes `PrintConfig.h` before defining **`P_PRINT_SPACE`**. If you do not define **`P_PRINT_SPACE`** there, the default is **`printf(" ")`.**

Edit **`Log/PrintConfig.h`** for a global separator, for example:

```c
/* PrintConfig.h */
#define P_PRINT_SPACE() printf("-")
```

You can also **`#define P_PRINT_SPACE`** before **`#include "Print.h"`** in a single file, or use a compiler **` -D`** if your toolchain allows defining function-like macros that way. After **`Print.h`** has already applied its default, use **`#undef P_PRINT_SPACE`** then **`#define`** to override locally.

## Examples

Basic print:

```cpp
#include "Print.h"

PRINTLN("answer", 42);   /* answer 42 */
PRINTLN("x =", 3.14);    /* x = 3.14 */
PRINT("x =", 3.14, "y =", 5.65);    /* x = 3.14 y = 5.65 (no newline) */
```

`PRINTLN(...)` is `PRINT(..., "\n")`.

Print with a custom separator (per translation unit, after `Print.h` has set the default):

```cpp
#include "Print.h"

#undef P_PRINT_SPACE
#define P_PRINT_SPACE()
PRINTLN("answer", 42);   /* answer42 */

#undef P_PRINT_SPACE
#define P_PRINT_SPACE() printf("-")
PRINTLN("x", 3.14, "y", 5.65);    /* x-3.14-y-5.65 */
```

Prefer **`PrintConfig.h`** when the same separator should apply everywhere (see above).

## Usage

- **`PRINT(...)`** — Up to 8 arguments, printed in order with **`P_PRINT_SPACE()`** between values. No trailing newline.
- **`PRINTLN(...)`** — Same as **`PRINT`** but appends a newline.
- **`P_PRINT_SPACE()`** — Invoked between arguments; default **`printf(" ")`** unless set in **`PrintConfig.h`** or overridden as above.

## Implementation notes

- **C** — `_Generic` dispatches to `printf` for: `int`, `unsigned`, `long`, `double`, `float`, `char`, `char *`, `const char *`, `void *`. Other types hit the `default` branch (error message on stderr). Clang/GCC format diagnostics are suppressed around the generic `printf` calls.
- **C++** — Uses `std::cout <<` for each value, so any type with a suitable `operator<<` works.

# Log

## Configuration (`LogConfig.h`)

`Log.h` includes `LogConfig.h` after defining `DEBUG`, `INFO`, `WARN`, `ERROR`, and `FATAL`, so you can use those names when setting **`LOG_LEVEL`**.

Edit **`Log/LogConfig.h`** to set options project-wide (see the comments in that file for **`LOG_LEVEL`**, **`LOG_DISABLE`**, and custom levels):

```c
/* LogConfig.h */
#define LOG_LEVEL WARN
/* #define LOG_DISABLE */
/* optional: CUSTOM level + P_LOG_CUSTOM + LOG_CUSTOM(...) */
```

Equivalently, **`#define LOG_LEVEL`** / **`LOG_DISABLE`** in a translation unit **before** `#include "Log.h"`, or pass **`-DLOG_LEVEL=2`**, **`-DLOG_DISABLE`** on the compiler command line. For **`LOG_LEVEL`**, use a level name (`WARN`, …) or a numeric value matching the level constants.

## Examples

Basic log:

```cpp
#include "Log.h"

int status = 1;
LOG_DEBUG("status:", status);
LOG(INFO, "status ok:", status == 1);
```

Typical level filtering (only **`LOG_LEVEL`** affects the `LOG_*` shortcuts; pick one way to set it):

```cpp
/* In LogConfig.h: */
#define LOG_LEVEL WARN
/* Keeps WARN, ERROR, FATAL; DEBUG and INFO are compiled out. */

/* Or before include / via -D: */
#define LOG_LEVEL FATAL
/* Only FATAL remains. */

#define LOG_LEVEL FATAL + 1
/* All built-in shortcuts compiled out; keep custom LOG_<NAME>(...) you define. */
```

Disable only the generic **`LOG(level, ...)`** macro (shortcuts like **`LOG_INFO`** still behave according to **`LOG_LEVEL`**):

```cpp
#define LOG_DISABLE
/* Either in LogConfig.h or before #include "Log.h" / -DLOG_DISABLE */
```

Custom log level (green prefix). Define **`P_LOG_<NAME>`** for the ANSI prefix and **`LOG_<NAME>`** using **`P_LOG_IMPL`** so **`LOG(NAME, ...)`** resolves:

```cpp
#define CUSTOM
#define P_LOG_CUSTOM "\033[32m"
#define LOG_CUSTOM(...) P_LOG_IMPL(CUSTOM, __VA_ARGS__)

LOG(CUSTOM, "hello");
LOG_CUSTOM("hello");
```

## Usage

- **`LOG(level, ...)`** — Expands to `LOG_##level(__VA_ARGS__)`. Message body uses **`PRINT`** (see above).
- **Shortcuts** — `LOG_DEBUG`, `LOG_INFO`, `LOG_WARN`, `LOG_ERROR`, `LOG_FATAL` call **`P_LOG_IMPL`** for that level.
- **Standard levels** — `DEBUG`, `INFO`, `WARN`, `ERROR`, `FATAL`: white, blue, orange, red, red background (see **`P_LOG_*`** in `Log.h`).
- **`LOG_LEVEL`** — Default is **`DEBUG`** if unset when `Log.h` is processed. Levels numerically _greater than_ **`LOG_LEVEL`** are compiled out (`DEBUG` is 0, `INFO` 1, …). Use **`FATAL + 1`** to remove all built-in shortcuts while keeping custom **`LOG_<NAME>(...)`** macros.
- **`LOG_DISABLE`** — If defined when `Log.h` is included, **`LOG(level, ...)`** expands to nothing. **`LOG_DEBUG`**, **`LOG_INFO`**, etc. are unchanged (still subject to **`LOG_LEVEL`**). Custom macros that call **`P_LOG_IMPL`** are not affected.
