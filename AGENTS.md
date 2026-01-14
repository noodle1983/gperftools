# gperftools Agent Guide

This document provides essential information for AI agents working with the gperftools codebase.

## Build System

gperftools uses multiple build systems:
- **Autotools** (primary): `configure.ac`, `Makefile.am`
- **CMake**: `CMakeLists.txt`
- **Bazel**: `BUILD.bazel`, `MODULE.bazel`

### Common Build Commands

```bash
# Configure and build with autotools
./autogen.sh
./configure
make

# Build with CMake (from build directory)
mkdir build && cd build
cmake ..
make

# Run all tests
make check
# or with CMake build
cd build && ctest

# Run a specific test
cd build && ./tcmalloc_unittest
cd build && ./stacktrace_unittest
# etc. (see build directory for all test binaries)

# Clean build
make clean
# or with CMake
cd build && make clean
```

### Test Commands

Tests are located in `src/tests/`. Each test compiles to a standalone executable in the build directory.

```bash
# Run a single test
./build/tcmalloc_unittest
./build/stacktrace_unittest
./build/heap-profiler_unittest

# Run test with script wrapper (some tests have shell scripts)
./src/tests/profiler_unittest.sh ./build

# Run all tests via CTest
cd build && ctest

# Run tests with verbose output
cd build && ctest -V

# Run specific test with CTest
cd build && ctest -R tcmalloc_unittest
```

## Code Style Guidelines

### File Headers
All source files must start with the standard copyright header:
```cpp
// -*- Mode: C++; c-basic-offset: 2; indent-tabs-mode: nil -*-
// Copyright (c) 2005, Google Inc.
// All rights reserved.
//
// [BSD license text...]
```

### Naming Conventions
- **Macros**: `UPPER_CASE_WITH_UNDERSCORES`
- **Constants**: `kConstantName`
- **Types**: `CamelCase` (classes, structs, typedefs)
- **Functions**: `CamelCase()` or `snake_case()` (mixed usage in codebase)
- **Variables**: `snake_case`
- **Private members**: `snake_case_` (trailing underscore)
- **File names**: `snake_case.{h,cc}`

### Namespaces
- Primary namespace: `tcmalloc`
- Use `namespace tcmalloc { ... }` for internal implementations
- Avoid `using namespace` directives in headers
- Close namespaces with `}  // namespace tcmalloc`

### Header Guards
Use `#ifndef` style guards with project prefix:
```cpp
#ifndef TCMALLOC_FILENAME_H_
#define TCMALLOC_FILENAME_H_
// ...
#endif  // TCMALLOC_FILENAME_H_
```

### Includes Order
1. Corresponding header
2. C system headers
3. C++ standard library headers
4. Other libraries' headers
5. Project headers

Example:
```cpp
#include "config.h"
#include <stddef.h>
#include <stdint.h>
#include <vector>
#include "base/basictypes.h"
#include "internal_logging.h"
```

### Formatting
- **Indentation**: 2 spaces (no tabs)
- **Line length**: ~80 characters (flexible)
- **Braces**: K&R style for functions, Allman style for classes
- **Pointer/Reference alignment**: `Type* ptr`, `Type& ref`

### Error Handling
- Use `ASSERT()` for internal invariants (debug builds only)
- Use `CHECK()` for runtime checks (always enabled)
- Return error codes or use `abort()` for fatal errors
- Document error conditions in function comments

### Memory Management
- Use `malloc()`, `free()` for C interfaces
- Use `new`, `delete` for C++ (with proper exception handling)
- Follow RAII principles for resource management
- Use `PERFTOOLS_NOTHROW` macro for functions that shouldn't throw

### Thread Safety
- Document thread safety assumptions
- Use `base::SpinLock` for low-level synchronization
- Follow existing patterns for per-thread caching
- Mark thread-safe functions with comments

### Platform Portability
- Use `config.h` for platform-specific definitions
- Use `#ifdef` for platform-specific code
- Follow existing patterns in `src/windows/` and `src/base/`
- Test on multiple platforms when making changes

## Project Structure

```
src/
├── base/           # Low-level utilities (spinlock, sysinfo, etc.)
├── gperftools/     # Public headers
├── tests/          # Test files
├── windows/        # Windows-specific code
└── *.cc, *.h       # Core implementation
```

### Key Directories
- `src/base/`: Platform-independent utilities
- `src/tests/`: Unit tests (one `.cc` file per test)
- `src/windows/`: Windows portability layer
- `vendor/`: Third-party dependencies (googletest, libbacktrace)

## Testing Guidelines

### Writing Tests
1. Use Google Test framework (`#include "gtest/gtest.h"`)
2. Place tests in `src/tests/` directory
3. Follow naming: `*_unittest.cc` or `*_test.cc`
4. Test both normal and edge cases
5. Include stress tests for performance-critical code

### Test Patterns
```cpp
#include "gtest/gtest.h"
#include "config_for_unittests.h"

TEST(TcmallocTest, BasicAllocation) {
  void* ptr = malloc(1024);
  ASSERT_NE(ptr, nullptr);
  free(ptr);
}
```

### Running Tests
- Build tests with `make` or `cmake --build`
- Run individual tests from build directory
- Use `ctest` for batch execution
- Some tests require shell scripts (check `src/tests/*.sh`)

## Common Patterns

### Allocation Functions
```cpp
void* tc_malloc(size_t size) PERFTOOLS_NOTHROW;
void tc_free(void* ptr) PERFTOOLS_NOTHROW;
```

### Internal Logging
```cpp
#include "internal_logging.h"
LOG(kLog, "Message: %d", value);
ASSERT(condition);
CHECK(condition);
```

### Platform Abstraction
```cpp
#include "config.h"
#ifdef HAVE_SOME_FEATURE
  // Platform-specific code
#endif
```

## Development Workflow

1. **Understand the change**: Review existing similar code
2. **Write tests first**: Add or update tests in `src/tests/`
3. **Implement change**: Follow existing patterns and style
4. **Build and test**: `make && make check`
5. **Verify no regressions**: Run relevant test suites
6. **Check formatting**: Ensure consistent style

## Important Notes

- **No Cursor/Copilot rules** found in this repository
- **Vendor code**: Don't modify files in `vendor/` directory
- **Public API**: Be careful with changes in `src/gperftools/`
- **Backward compatibility**: Maintain API/ABI compatibility
- **Performance**: This is a performance tools library - avoid regressions

## Troubleshooting

### Common Issues
- **Missing config.h**: Run `./configure` or `cmake` first
- **Test failures**: Check environment variables (HEAPPROFILE, CPUPROFILE)
- **Build errors**: Ensure all dependencies are installed
- **Portability issues**: Test on target platforms

### Debug Builds
```bash
./configure --enable-debug
# or
cmake -DCMAKE_BUILD_TYPE=Debug ..
```

### Memory Debugging
```bash
# Enable debug allocation
export TCMALLOC_DEBUG=1
# Run with heap checker
export HEAPCHECK=strict
```

This guide should help agents navigate and contribute to the gperftools codebase effectively.