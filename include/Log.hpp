#pragma once

#include <string>
#include <mutex>
#include <ostream>

class Log {
private:
    class LogStreamWrapper {
    private:
        std::mutex& m_lock;
        std::ostream& m_stream;

    public:
        LogStreamWrapper(std::mutex& lock, std::ostream& stream);
        ~LogStreamWrapper();

        template<class T>
        LogStreamWrapper& operator<<(const T& data) {
            m_stream << data;
            return *this;
        }
    };

    std::mutex m_lock;
    std::ostream& m_stream;

    Log(std::ostream& stream);

    static Log& instance();
    static LogStreamWrapper message(const std::string& level, const std::string& scope);

public:
    static LogStreamWrapper info(const std::string& scope);
    static LogStreamWrapper warning(const std::string& scope);
    static LogStreamWrapper fatal(const std::string& scope);
};
