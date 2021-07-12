#include "orpheus/async/Executor.hpp"

namespace orpheus::async {
    Executor::Executor(const std::string& name) :
        m_running(true),
        m_name(name)
    { }

    Executor::~Executor() {
        {
            std::unique_lock lock(m_mutex);
            m_running = false;
            m_condvar.notify_all();
        }
        if (m_worker && m_worker->joinable()) {
            m_worker->join();
        }
    }

    void Executor::start() {
        m_worker = std::make_shared<std::thread>([this]() { doWork(); });
    }

    std::future<void> Executor::execute(const Function& function) {
        std::unique_lock lock(m_mutex);
        m_tasks.emplace_back(Task{function, std::promise<void>()});
        m_condvar.notify_all();

        return m_tasks.back().promise.get_future();
    }

    void Executor::doWork() {
        std::unique_lock lock(m_mutex);
        while (m_running) {
            if (m_tasks.size() == 0) {
                m_condvar.wait(lock);
                continue;
            }

            auto& task = m_tasks.front();
            task.function();
            task.promise.set_value();
            m_tasks.pop_front();
        }
    }
}
