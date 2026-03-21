# CppUtils

Small, header-friendly C++ utilities and demos. Each folder is self-contained: include the headers you need and compile the matching `main.cpp` if you want the sample program.

## Modules

| Module                | Purpose                                                                                                                  |
| --------------------- | ------------------------------------------------------------------------------------------------------------------------ |
| [Random](Random/)     | `Random` + `RANDOM_INSTANCE` with `range<T>(min, max)` and typed generators (`next()`).                                  |
| [Inquirer](Inquirer/) | Terminal prompts: `input`, `select`, `multiselect`, `confirm` (see [Inquirer.hpp](Inquirer/Inquirer.hpp)).               |
| [Assert](Assert/)     | `ASSERT(condition, message)` — prints location and exits on failure ([Assert.h](Assert/Assert.h)).                       |
| [Log](Log/)           | Colored `LOG(level, ...)` macros with compile-time level filtering via `LogConfig.h` / `LOG_LEVEL` ([Log.h](Log/Log.h)). |

## Building the demos

From the repo root, examples (adjust compiler and C++ standard as needed):

```bash
g++ -std=c++20 -Wall -Wextra -pedantic -I Random Random/main.cpp -o random_demo
g++ -std=c++20 -Wall -Wextra -pedantic -I Inquirer Inquirer/main.cpp Inquirer/Inquirer.cpp -o inquirer_demo
g++ -std=c++20 -Wall -Wextra -pedantic -I Assert Assert/main.cpp -o assert_demo
g++ -std=c++20 -Wall -Wextra -pedantic -I Log Log/main.cpp -o log_demo
```

- **Assert demo**: `assert_demo pass` or `assert_demo fail` (or `-h`).
- **Log demo**: `log_demo demo` or `log_demo levels` (or `-h`).

## Editor / static analysis

[compile_flags.txt](compile_flags.txt) lists warning flags for **clangd** (see [.clangd](.clangd)). It is not a full build definition.

## Examples

**Random** — from [Random/main.cpp](Random/main.cpp):

```cpp
auto intGen = RANDOM_INSTANCE.range<int>(1, 10);
std::cout << intGen.next() << '\n';

auto floatGen = RANDOM_INSTANCE.range<float>(0.0F, 1.0F);
std::cout << floatGen.next() << '\n';
```

**Inquirer** — call `enableAnsiTerminal()` once, then use the static helpers; full flow in [Inquirer/main.cpp](Inquirer/main.cpp).

```cpp
enableAnsiTerminal();
std::string name = Inquirer::input("Project name", "my-app");
std::string framework = Inquirer::select("Choose framework", {"React", "Vue"}, 0);
```

**Assert** — from [Assert/main.cpp](Assert/main.cpp):

```cpp
ASSERT(argc > 1, "expected subcommand");
```

**Log** — from [Log/main.cpp](Log/main.cpp):

```cpp
LOG(INFO, "info line", "10:", 10);
LOG(DEBUG, "debug line");
LOG(WARN, "warning line");
```
