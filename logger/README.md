# Print / Log

Small headers only: include `print.h` for type-friendly printing, or `logger.h` for leveled logs (`logger.h` includes `print.h`). Project-wide tweaks go in **`print_config.h`** (included by `print.h` after the standard library headers) and **`logger_config.h`** (included by `logger.h` after `DEBUG` … `FATAL` are defined).

# Print

## Configuration (`print_config.h`)

`print.h` includes `print_config.h` before defining **`P_PRINT_SPACE`**. If you do not define **`P_PRINT_SPACE`** there, the default is **`printf(" ")`.**

Edit **`logger/print_config.h`** for a global separator, for example:

```c
/* print_config.h */
#define P_PRINT_SPACE() printf("-")
```

You can also **`#define P_PRINT_SPACE`** before **`#include "print.h"`** in a single file, or use a compiler **`-D`** if your toolchain allows defining function-like macros that way. After **`print.h`** has already applied its default, use **`#undef P_PRINT_SPACE`** then **`#define`** to override locally.

## Examples

Basic print:

```cpp
#include "print.h"

PRINT_LN("answer", 42);   /* answer 42 */
PRINT_LN("x =", 3.14);    /* x = 3.14 */
PRINT("x =", 3.14, "y =", 5.65);    /* x = 3.14 y = 5.65 (no newline) */
```

`PRINT_LN(...)` is `PRINT(..., "\n")`. Empty `PRINT()` is undefined.

Print with a custom separator (per translation unit, after `print.h` has set the default):

```cpp
#include "print.h"

#undef P_PRINT_SPACE
#define P_PRINT_SPACE()
PRINT_LN("answer", 42);   /* answer42 */

#undef P_PRINT_SPACE
#define P_PRINT_SPACE() printf("-")
PRINT_LN("x", 3.14, "y", 5.65);    /* x-3.14-y-5.65 */
```

Prefer **`print_config.h`** when the same separator should apply everywhere (see above).

## Usage

- **`PRINT(...)`** — Up to 8 arguments, printed in order with **`P_PRINT_SPACE()`** between values. No trailing newline.
- **`PRINT_LN(...)`** — Same as **`PRINT`** but appends a newline.
- **`P_PRINT_SPACE()`** — Invoked between arguments; default **`printf(" ")`** unless set in **`print_config.h`** or overridden as above.

## Implementation notes

- **C** — `_Generic` dispatches to `printf` for: `int`, `unsigned`, `long`, `double`, `float`, `char`, `char *`, `const char *`, `void *`. Other types hit the `default` branch (error message on stderr). Clang/GCC format diagnostics are suppressed around the generic `printf` calls.
- **C++** — Uses `std::cout <<` for each value, so any type with a suitable `operator<<` works.

# Log

## Configuration (`logger_config.h`)

`logger.h` includes `logger_config.h` after defining `DEBUG`, `INFO`, `WARN`, `ERROR`, and `FATAL`, so you can use those names when setting **`LOG_LEVEL`**.

The default **`LOG_LEVEL`** is **`DEBUG`** (`0`): all built-in shortcuts stay compiled in. This tree sets that in **`logger_config.h`** with **`SET_LOG_LEVEL_DEBUG`**. If you remove that define and do not set **`LOG_LEVEL`** or another **`SET_LOG_LEVEL_*`**, `logger.h` still falls back to **`DEBUG`**.

Edit **`logger/logger_config.h`** to change it project-wide (see the comments in that file for **`LOG_LEVEL`**, **`SET_LOG_LEVEL_*`**, **`LOG_DISABLE`**, and custom levels):

```c
/* logger_config.h — shipped default: */
#define SET_LOG_LEVEL_DEBUG
/* #define LOG_LEVEL WARN */
/* #define SET_LOG_LEVEL_ERROR */
/* #define LOG_DISABLE */
/* optional: CUSTOM level + P_LOG_CUSTOM + LOG_CUSTOM(...) */
```

If **`LOG_LEVEL`** is unset, you can pick a level with a single define (useful as **`-DSET_LOG_LEVEL_WARN`**). Only the highest of these that is defined applies: **`SET_LOG_LEVEL_FATAL`**, **`SET_LOG_LEVEL_ERROR`**, **`SET_LOG_LEVEL_WARN`**, **`SET_LOG_LEVEL_INFO`**, **`SET_LOG_LEVEL_DEBUG`**.

Equivalently, **`#define LOG_LEVEL`** / **`LOG_DISABLE`** / **`SET_LOG_LEVEL_*`** in a translation unit **before** `#include "logger.h"`, or pass **`-DLOG_LEVEL=2`**, **`-DLOG_DISABLE`**, **`-DSET_LOG_LEVEL_ERROR`** on the compiler command line. For **`LOG_LEVEL`**, use a level name (`WARN`, …) or a numeric value matching the level constants.

## Path prefix

Each log line prints `file:line`. Call **`INIT_LOG_PATH_OFFSET()`** once at startup (needs **`strlen`**, so include `<string.h>` / `<cstring>`) to strip the directory prefix from `__FILE__` using **`__FILE_NAME__`** (GCC/Clang). Until then, the offset is 0 and the full path is shown.

```cpp
#include "logger.h"
#include <cstring>

int main()
{
	INIT_LOG_PATH_OFFSET();
	LOG_INFO("ready");
}
```

## Examples

Basic log:

```cpp
#include "logger.h"

int status = 1;
LOG_DEBUG("status:", status);
LOG(INFO, "status ok:", AS_BOOL(status == 1));
```

Typical level filtering (only **`LOG_LEVEL`** affects the `LOG_*` shortcuts; pick one way to set it):

```cpp
/* In logger_config.h: */
#define LOG_LEVEL WARN
/* Keeps WARN, ERROR, FATAL; DEBUG and INFO are compiled out. */

/* Or a SET_LOG_LEVEL_* flag: */
#define SET_LOG_LEVEL_FATAL
/* Only FATAL remains. */

#define LOG_LEVEL FATAL + 1
/* All built-in shortcuts compiled out; keep custom LOG_<NAME>(...) you define. */
```

Disable only the generic **`LOG(level, ...)`** macro (shortcuts like **`LOG_INFO`** still behave according to **`LOG_LEVEL`**):

```cpp
#define LOG_DISABLE
/* Either in logger_config.h or before #include "logger.h" / -DLOG_DISABLE */
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

- **`LOG(level, ...)`** — Expands to `LOG_##level(__VA_ARGS__)`. Message body uses **`PRINT`** (see above). Each line is `file:line: ` plus the message, with an ANSI color from **`P_LOG_*`** and a reset/newline at the end.
- **Shortcuts** — `LOG_DEBUG`, `LOG_INFO`, `LOG_WARN`, `LOG_ERROR`, `LOG_FATAL` call **`P_LOG_IMPL`** for that level.
- **Standard levels** — `DEBUG`, `INFO`, `WARN`, `ERROR`, `FATAL`: white, blue, orange, red, red background (see **`P_LOG_*`** in `logger.h`).
- **`LOG_LEVEL`** — Default is **`DEBUG`** (`0`), via **`SET_LOG_LEVEL_DEBUG`** in `logger_config.h`, or the fallback in `logger.h` if nothing is set. Levels numerically _greater than_ **`LOG_LEVEL`** are compiled out (`DEBUG` 0, `INFO` 1, `WARN` 2, `ERROR` 3, `FATAL` 4). Use **`FATAL + 1`** to remove all built-in shortcuts while keeping custom **`LOG_<NAME>(...)`** macros.
- **`LOG_DISABLE`** — If defined when `logger.h` is included, **`LOG(level, ...)`** expands to nothing. **`LOG_DEBUG`**, **`LOG_INFO`**, etc. are unchanged (still subject to **`LOG_LEVEL`**). Custom macros that call **`P_LOG_IMPL`** are not affected.
- **`AS_BOOL(x)`** — Prints a condition as `"true"` / `"false"`: `LOG_DEBUG("flag", AS_BOOL(flag))`.
- **`INIT_LOG_PATH_OFFSET()`** — Computes how much of `__FILE__` to skip so log lines show the basename.
