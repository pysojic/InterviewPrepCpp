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
#### C++

- [Effective Modern C++ — Scott Meyers](https://ananyapam7.github.io/resources/C++/Scott_Meyers_Effective_Modern_C++.pdf) — Item-based guidelines for C++11/14 that teach how to use move semantics, smart pointers, lambdas, auto, and concurrency correctly and idiomatically.

- [C++ Templates: The Complete Guide (2nd Edition) — David Vandevoorde, Nicolai M. Josuttis, Douglas Gregor](https://agorism.dev/book/c%2B%2B/David%20Vandevoorde%2C%20Nicolai%20M.%20Josuttis%2C%20Douglas%20Gregor%20-%20C%2B%2B%20Templates_%20The%20Complete%20Guide-Addison-Wesley%20%282017%29.pdf) — Definitive deep dive into templates, from basic syntax to type traits and template metaprogramming, essential for understanding modern generic C++ code.

- [The C++ Programming Language (3rd Edition) — Bjarne Stroustrup](https://lmu.web.psi.ch/docu/manuals/software_manuals/ISO_Cpp/Cpp-Bjarne-Stroustrup.pdf) — Comprehensive tour of the C++ language and standard library from its creator, ideal as a foundational reference and big-picture overview.

- [C++ High Performance, Second Edition: Master the art of optimizing the functioning of your C++ code](https://www.amazon.com.be/High-Performance-Second-optimizing-functioning/dp/1839216549) — Focuses on writing cache-friendly, branch-efficient, and scalable C++ code, with practical coverage of profiling, memory behavior, concurrency, and low-level tuning.

- [Beautiful C++](https://ptgmedia.pearsoncmg.com/images/9780137647842/samplepages/9780137647842_Sample.pdf) — Emphasizes clear, expressive, and maintainable modern C++ design, showing how to combine language features and idioms to produce elegant, robust code.

- [C++ Software Design](https://api.pageplace.de/preview/DT0400.9781098113131_A49444872/preview-9781098113131_A49444872.pdf) — A systems-level look at structuring C++ codebases: interfaces, boundaries, testability, and architecture patterns tailored to large C++ projects.


#### Computer Architecture and Operating Systems

- [Computer Architecture: A Quantitative Approach (6th Edition)](https://unidel.edu.ng/focelibrary/books/COMPUTER%20ARCHICTECTURE.pdf) — Classic reference on CPU and system design, using real quantitative data to explore pipelining, caches, memory hierarchies, and parallelism.

- [Computer Systems: A Programmer's Perspective (2nd Edition)](https://shuyuej.com/books/Computer%20Systems-%20A%20Programmer's%20Perspective.pdf) — Bridges the gap between hardware, compilers, and OSes, explaining how data representation, the memory hierarchy, linking, and concurrency affect real programs.

- [Operating Systems: Three Easy Pieces](https://www.amazon.com.be/-/en/Operating-Systems-Three-Easy-Pieces/dp/198508659X) — Concept-driven OS textbook organized around virtualization, concurrency, and persistence, with approachable explanations and hands-on projects.


#### Networking

- [TCP/IP Illustrated, Volume 1: The Protocols (2nd Edition)](http://www.r-5.org/files/books/computers/internals/net/Richard_Stevens-TCP-IP_Illustrated-EN.pdf) — Packet-level exploration of the TCP/IP stack, illustrating how protocols like IP, TCP, and UDP really behave on the wire with detailed traces and examples.

- [Computer Networks: A Systems Approach](https://book.systemsapproach.org/) — A systems-oriented networking textbook that explains the Internet stack from links and switching up through routing, congestion control, and applications, with a strong focus on design principles, trade-offs, and real-world protocol behavior.


#### Concurrency

- [Concurrency in Action — Anthony Williams (2nd Edition)](https://beefnoodles.cc/assets/book/C++%20Concurrency%20in%20Action.pdf) — The go-to guide for C++ concurrency primitives (threads, mutexes, atomics, futures) and patterns for writing correct, modern multithreaded C++ code.

- [High Performance Parallelism Pearls Volume One: Multicore and Many-core Programming Approaches](https://www.amazon.nl/High-Performance-Parallelism-Pearls-One/dp/0128021187) — Collection of case studies on extracting performance from multicore CPUs and accelerators, covering practical parallelization techniques and real-world workloads.

- [Concurrency with Modern C++](https://www.amazon.com.be/Concurrency-Modern-professional-programmer-concurrency/dp/B0CT41Q1FF) — Modern C++11–20 treatment of concurrency and parallelism, including threads, tasks, futures, lock-free techniques, and higher-level patterns for scalable systems.


#### Algorithms

- [Introduction to Algorithms (3rd Edition)](https://www.cs.mcgill.ca/~akroit/math/compsci/Cormen%20Introduction%20to%20Algorithms.pdf) — Comprehensive algorithms textbook (CLRS) covering data structures, graph algorithms, dynamic programming, and complexity analysis with mathematically rigorous treatments.


### Blogs

- [Herb Sutter’s Blog](https://herbsutter.com/)
  — Language evolution and modern best practices.

- [Agner Fog's Blog](https://www.agner.org/optimize/#manuals)
  — A bit on the advanced side. Contains a plethora of tips/advices for low-level optimization.

- [rigtorp.se](https://rigtorp.se/)
  — A blog by Erik Rigtorp featuring deep dives into performance, benchmarking, data structures, and low-level C++ techniques.

- [Johnny’s SW Lab – Performance](https://johnnysswlab.com/category/performance/)
  — Blog posts focused on software performance, profiling techniques, and low-level optimization in C++ and systems development.

- [Preshing – Blog](https://preshing.com/archives/)
  — A blog by Jeff Preshing covering algorithms, lock-free data structures, concurrency, and systems-level programming insights.

### Articles and Papers
- [What Every Programmer Should Know About Memory](https://lwn.net/Articles/250967/)
  — Ulrich Drepper’s classic deep dive into RAM, CPU caches, NUMA, and how modern memory hierarchies affect performance and correctness. A must-read if you care about writing cache-friendly, high-performance C/C++ and systems code.

- [What Every Computer Scientist Should Know About Floating-Point Arithmetic](https://www.itu.dk/~sestoft/bachelor/IEEE754_article.pdf)
  — David Goldberg’s definitive tutorial on IEEE 754, rounding, cancellation, and numerical error. Essential to understand why floats “behave weirdly” and how to write numerically robust code.

- [IEEE754](https://people.eecs.berkeley.edu/~wkahan/ieee754status/IEEE754.PDF)
  — William Kahan’s lecture notes on the design and status of the IEEE 754 floating-point standard, with historical context, rationale, and many subtle corner cases from the person most associated with the standard.

- [C++ Design Patterns for Low-latency Applications Including High-frequency Trading](https://arxiv.org/abs/2309.04259)
  — A research-driven overview of C++ design patterns and techniques for ultra-low-latency systems (e.g., HFT), covering cache-aware design, lock-free patterns, and practical performance tuning backed by benchmarks.

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
