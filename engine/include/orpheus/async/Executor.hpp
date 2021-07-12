#pragma once

#include <condition_variable>
#include <deque>
#include <functional>
#include <future>
#include <mutex>

namespace orpheus::async {
    class Executor {
    public:
        using Function = std::function<void()>;

    private:
        struct Task {
            Function           function;
            std::promise<void> promise;
        };

        bool                         m_running;
        std::string                  m_name;
        std::shared_ptr<std::thread> m_worker;
        std::deque<Task>             m_tasks;
        std::mutex                   m_mutex;
        std::condition_variable      m_condvar;

        void doWork();

    public:
        Executor(const std::string& name);
        ~Executor();

        void start();

        std::future<void> execute(const Function& function);
    };
}
