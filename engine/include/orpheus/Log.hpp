#pragma once

#include <string>
#include <mutex>
#include <ostream>
#include <iostream>
#include <iomanip>
#include <memory>

#include "Scope.hpp"

namespace Orpheus {
    class Log {
    private:
        class LogStreamWrapper {
        private:
            std::mutex& m_lock;
            std::ostream& m_stream;

        public:
            LogStreamWrapper(std::mutex& lock, std::ostream& stream) :
                m_lock(lock),
                m_stream(stream)
            {
                m_lock.lock();
                auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                auto tm = *std::gmtime(&tt);
                m_stream << std::put_time(&tm, "%Y-%m-%d %H:%M:%S ");
            }

            ~LogStreamWrapper() {
                m_stream << std::endl;
                m_lock.unlock();
            }

            template<class T>
            LogStreamWrapper& operator<<(const T& data) {
                m_stream << data;
                return *this;
            }
        };

        std::mutex m_lock;
        std::ostream& m_stream;

        Log(std::ostream& stream) :
            m_stream(stream)
        {
        }

        static Log& instance() {
            static Log log(std::cout);
            return log;
        }

        template<class T> struct remove_all_pointers                           { using type = T; };
        template<class T> struct remove_all_pointers<T*>                       { using type = T; };
        template<class T> struct remove_all_pointers<T* const>                 { using type = T; };
        template<class T> struct remove_all_pointers<std::shared_ptr<T>>       { using type = T; };
        template<class T> struct remove_all_pointers<std::shared_ptr<T> const> { using type = T; };

        template<
            class T,
            class S = typename remove_all_pointers<typename std::remove_reference<T>::type>::type
        >
        static LogStreamWrapper message(const std::string& level) {
            static_assert(std::is_base_of<Scoped, S>::value, "S should be derived from Orpheus::Scoped");
            auto& self = instance();

            LogStreamWrapper wrapper(self.m_lock, self.m_stream);

            wrapper << "| " << level << " | ";
            bool first = true;
            for (const auto& scope : S::getScopes()) {
                if (first) {
                    first = false;
                } else {
                    wrapper << "::";
                }
                wrapper << scope;
            }
            wrapper << ": ";

            return wrapper;
        }

    public:
        template<class T>
        static LogStreamWrapper info(const T& scoped) {
            return Log::message<T>(" INFO");
        }

        template<class T>
        static LogStreamWrapper warning(const T& scoped) {
            return Log::message<T>(" WARN");
        }

        template<class T>
        static LogStreamWrapper fatal(const T& scoped) {
            return Log::message<T>("FATAL");
        }
    };
}
