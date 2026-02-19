#include <gtest/gtest.h>
#include "threadpool.hpp"

TEST(ThreadPool, BasicExecution) {
    ThreadPool pool(4);
    auto f = pool.enqueue([] { return 123; });
    EXPECT_EQ(f.get(), 123);
}

TEST(ThreadPool, ManyTasks) {
    ThreadPool pool(4);

    constexpr int N = 10000;
    std::atomic<int> counter = 0;

    std::vector<std::future<void>> futures;

    for (int i = 0; i < N; ++i) {
        futures.push_back(pool.enqueue([&counter] {
            counter++;
        }));
    }

    for (auto& f : futures)
        f.get();

    EXPECT_EQ(counter.load(), N);
}

TEST(ThreadPool, ParallelExecution) {
    ThreadPool pool(4);

    auto start = std::chrono::steady_clock::now();

    std::vector<std::future<void>> futures;

    for (int i = 0; i < 4; i++) {
        futures.push_back(pool.enqueue([] {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }));
    }

    for (auto &f : futures)
        f.get();

    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    EXPECT_LT(duration.count(), 1000);
    std::cout << "Elapsed duration: " << duration.count() << " ms";
}

TEST(ThreadPool, ExceptionPropagation) {
    ThreadPool pool(2);

    auto future = pool.enqueue([]() -> int {
        throw std::runtime_error("error");
    });

    EXPECT_THROW(future.get(), std::runtime_error);
}

TEST(ThreadPool, GracefulShutdown) {
    {
        ThreadPool pool(4);

        for (int i = 0; i < 100; ++i) {
            pool.enqueue([] {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            });
        }
    }
    SUCCEED();  // If destructor hangs, test will timeout
}

TEST(ThreadPool, EnqueueAfterShutdownThrows) {
    ThreadPool pool(2);
    pool.shutdown();

    EXPECT_THROW(pool.enqueue([] {}), std::runtime_error);
}

TEST(ThreadPool, WorkerBounding) {
    ThreadPool pool(4);

    std::vector<std::future<void>> futures;

    for (int i = 0; i < 5; i++) {
        futures.push_back(pool.enqueue([] {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }));
        EXPECT_EQ(pool.size(), 4);
    }

    for (auto &f : futures)
        f.get();
        EXPECT_EQ(pool.size(), 4);
}

