#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t numThreads) : stop(false) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back([this] {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock<std::mutex> lock(this->tasks_queue_mutex);
                    this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                    if (this->stop && this->tasks.empty()) {
                        return;
                    }
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }
                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::unique_lock<std::mutex> lock(tasks_queue_mutex);
        stop = true;
    }
    condition.notify_all();
    for (std::thread& worker : workers) {
        worker.join();
    }
}


const size_t ThreadPool::GetCurrentWorkers() const {
    return workers.size();
}

const size_t ThreadPool::GetCurrentTasks() const {
    return tasks.size();
}

void ThreadPool::CancelAllCurrentTasks()
{
    std::unique_lock<std::mutex> lock(tasks_queue_mutex);
    for (; !tasks.empty(); tasks.pop()) {}
}


