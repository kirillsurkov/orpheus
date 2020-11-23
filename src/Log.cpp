#include "Log.hpp"
#include <iostream>
#include <iomanip>

Log::LogStreamWrapper::LogStreamWrapper(std::mutex& lock, std::ostream& stream) :
    m_lock(lock),
    m_stream(stream)
{
    m_lock.lock();
    auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    auto tm = *std::gmtime(&tt);
    m_stream << std::put_time(&tm, "%Y-%m-%d %H:%M:%S ");
}

Log::LogStreamWrapper::~LogStreamWrapper() {
    m_stream << std::endl;
    m_lock.unlock();
}

Log::Log(std::ostream& stream) :
    m_stream(stream)
{
}

Log& Log::instance() {
    static Log log(std::cout);
    return log;
}

Log::LogStreamWrapper Log::message(const std::string& level, const std::string& scope) {
    auto& self = instance();
    LogStreamWrapper wrapper(self.m_lock, self.m_stream);
    wrapper << level << " " << scope << " ";
    return wrapper;
}

Log::LogStreamWrapper Log::info(const std::string& scope) {
    return Log::message("INFO   ", scope);
}

Log::LogStreamWrapper Log::warning(const std::string& scope) {
    return Log::message("WARNING", scope);
}

Log::LogStreamWrapper Log::fatal(const std::string& scope) {
    return Log::message("FATAL  ", scope);
}
