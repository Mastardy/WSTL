# WSTL

A custom C++ Standard Template Library implementation, built for use in a personal game engine.

## Overview

WSTL is a header-only library providing containers, smart pointers, and utility types as alternatives to the C++ standard library. It is developed alongside a Visual Studio solution with a sandbox project and a unit test suite.

## Features

- **Containers** — `Array`, `Vector`, `FixedVector`, `Deque`, `List`, `SList`, `Stack`, `Queue`, `PriorityQueue`, `BitSet`, `Pair`
- **Associative** — `Map`, `Set`, `HashMap`, `RBTree`, `BinaryHeap`
- **Memory** — `Allocator`, `UniquePointer`, `SharedPointer`, `WeakPointer`
- **Utility** — `Any`, `Optional`, `Hash`, `TypeTraits`

## Usage

Include the umbrella header:

```cpp
#include "WSTL/WSTL.hpp"
```

Or pull in individual components as needed (e.g. `WSTL/containers/Vector.hpp`).

## Building

Open `WSTL.sln` in Visual Studio. The solution contains three projects:

- `WSTL` — the header-only library
- `Sandbox` — scratch executable for experimentation
- `UnitTestProject` — Google Test–based unit tests

## License

Released under the [MIT License](LICENSE.md).
