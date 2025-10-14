# InterviewPrepCpp

This repository is a modern C++ interview preparation toolkit, designed to deepen the understanding of core C++ concepts through clean, from-scratch implementations of data structures, concurrency primitives, smart pointers, metaprogramming techniques and more. It is organized to mirror the themes commonly explored in interviews, while encouraging a deeper grasp of language internals and performance considerations.

## Structure Overview

### Containers
Custom STL-like implementations that exercise memory management, iterators, and algorithmic behavior:
- `Array`, `Vector`, `String`
- `List`, `ForwardList`
- `HashMap` (chaining) and `OpenAddressingHashMap`
- `SPSCQueue` for single-producer/single-consumer scenarios

### Concurrency
Basic synchronization primitives implemented manually to understand low-level threading:
- `Mutex` (POSIX-based)
- `SpinLock` (busy-wait locking)

### SmartPointers
Custom smart pointer implementations that mimic `unique_ptr`, `shared_ptr`, and related semantics.  
- `UniquePtr`
- `SharedPtr`
- `WeakPtr`

### Sorting
Collection of classical sorting algorithms (e.g., quicksort, mergesort, heapsort) implemented with a focus on algorithmic clarity and performance trade-offs.

### Utilities
Miscellaneous utilities showcasing advanced language features:
- `Any.hpp`: type-erasure container
- `CompileTimeFunctions.hpp`: template metaprogramming and constexpr exploration
- `move_semantics.hpp`: move, forward utility functions

## Purpose

This project is not meant to replace the STL. Instead, it is a learning environment to:

- Reinforce your understanding of core data structures by re-implementing them in a simple way.
- Explore concurrency and memory models at a low level.
- Practice template metaprogramming and compile-time techniques.
- Build interview-ready intuition through hands-on coding rather than rote memorization.

## Build Instructions

```bash
cmake -S . -B build
cmake --build build
```

You can include individual headers in your own C++ test files or use the provided tests.hpp for quick experimentation.

## References & Further Reading

This section gathers useful resources — books, blogs, and documentation — that complement the topics covered in this repository. It serves as a quick reference for deepening your understanding of C++ internals, algorithms, and interview patterns.

### Books

- [Effective Modern C++ — Scott Meyers](https://ananyapam7.github.io/resources/C++/Scott_Meyers_Effective_Modern_C++.pdf)

- [C++ Templates: The Complete Guide (2nd Edition) — David Vandevoorde, Nicolai M. Josuttis, Douglas Gregor](https://agorism.dev/book/c%2B%2B/David%20Vandevoorde%2C%20Nicolai%20M.%20Josuttis%2C%20Douglas%20Gregor%20-%20C%2B%2B%20Templates_%20The%20Complete%20Guide-Addison-Wesley%20%282017%29.pdf)

- [The C++ Programming Language (3rd Edition) — Bjarne Stroustrup](https://lmu.web.psi.ch/docu/manuals/software_manuals/ISO_Cpp/Cpp-Bjarne-Stroustrup.pdf)

- [Concurrency in Action — Anthony Williams](https://beefnoodles.cc/assets/book/C++%20Concurrency%20in%20Action.pdf)

### Blogs, Articles

- [Herb Sutter’s Blog](https://herbsutter.com/)
  — Language evolution and modern best practices.

- [Agner Fog's Blog](https://agner.org/)
  — A bit on the more advanced side. Contains a plethora of tips/advices for low-level optimization.

- [rigtorp.se](https://rigtorp.se/)
  — A blog by Erik Rigtorp featuring deep dives into performance, benchmarking, data structures, and low-level C++ techniques.

- [Johnny’s SW Lab – Performance](https://johnnysswlab.com/category/performance/)
  — Blog posts focused on software performance, profiling techniques, and low-level optimization in C++ and systems development.

- [Preshing – Blog](https://preshing.com/archives/)
  — A blog by Jeff Preshing covering algorithms, lock-free data structures, concurrency, and systems-level programming insights.

### Documentation & Useful Websites

- [CppReference](https://cppreference.com/)
  — Authoritative reference for the standard library and language features.
  
- [LINUX Man Pages](https://www.kernel.org/doc/man-pages/)
  — Official and comprehensive reference for Linux system calls, library functions, and interfaces.

- [ISO C++ Drafts](https://eel.is/c++draft/)
  — For exploring the language standard.

- [Compiler Explorer](https://godbolt.org/)
  — Visualize template instantiations, optimizations, and generated assembly.

- [LearnCpp](https://www.learncpp.com/)
  — Good place to start reviewing basic C++ concepts.

- [getcracked.io](https://www.getcracked.io)
  — Repository of technical interview questions spanning data structures, algorithms, system design, and more.
