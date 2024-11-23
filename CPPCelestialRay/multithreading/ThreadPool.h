#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <future>

class ThreadPool {
public:
    explicit ThreadPool(size_t numThreads);

    ~ThreadPool();

    template<class F, class ...Args>
    auto enqueue(F&& f, Args && ...args) -> std::future<typename std::invoke_result<F, Args ...>::type> {
        using return_type = typename std::invoke_result<F, Args...>::type;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex);

            // don't allow enqueueing after stopping the pool
            if (stop)
                throw std::runtime_error("enqueue on stopped ThreadPool");

            tasks.emplace([task]() { (*task)(); });
        }
        condition.notify_one();
        return res;
    }

    const size_t GetCurrentWorkers() const;

    const size_t GetCurrentTasks() const;

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
};

/*
auto task = [x]() -> int { return x * x; };
for (int i = 0; i < 10; ++i) {
    auto result = pool.enqueue(task, i);
    std::cout << "Result: " << result.get() << std::endl;
}*/

