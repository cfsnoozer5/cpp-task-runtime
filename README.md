# ThreadPool
This is a very basic threadpooling header-only library developed for educational purposes. 

This is not a fully fleshed out nor is it intended to be. DO NOT USE IN PRODUCTION.

## Motivation
This is an educational project, I'm quite familiar and have implemented futures in Rust, but I wanted to deepen my knowledge of CPP, the standard library and future management in CPP. Since CPP is also fairly widely utilzed across industries.
- Mutithreading and syncrhonization in C++
- Basic threadpooling api I can use in other learning projects.

## Features
- Fixed-size worker pool
- Task queue system and asynchornous exectuion
- Returnable tasks. So data can actually be utilized intstead of just executing logic
- Thread-safe enqueueing
- Exception-safe shutdown

## Learning Notes
This is a section mostly for me to look back on later to remember what I learned/what I can use later:
- condtion_variable useful syncing mechanism for cross thread notification. Sort of like what a Waker is in Rust. Will notify/wake up one or all threads to do work or terminate
- Predicate checking in the .wait to prevent issues where another worker has taken a task or useful for spurious wake ups when no tasks are in the queue.
- lvalue vs rvalue for function binding and why std::forward is necessary and why && is required in the bind definition so we can move if rvalue, but reference if lvalue.
- future is the asynchronous wrapper that a client will use by .get() to get that value back, like await in other languages sort of.

## Usage/Example
```
#include "threadpool.hpp"
#include <iostream>

int main() {
    ThreadPool pool(4);

    auto future = pool.enqueue([](int x){ return x*x; }, 5);

    std::cout << "Result: " << future.get() << "\n"; // prints 25

    // Enqueue multiple tasks
    std::vector<std::future<int>> results;
    for (int i = 0; i < 8; ++i) {
        results.push_back(pool.enqueue([i]{ return i*2; }));
    }
    for (auto &f : results)
        std::cout << f.get() << " ";
    std::cout << "\n";
}
```

## Testing
- GoogleTest
- Basic Testing Coverage:
    - Basic task execution
    - Mutiple task execution
    - Parallel Execution
    - Exception Handling
- test with 'ctest'
