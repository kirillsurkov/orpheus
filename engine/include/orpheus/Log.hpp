#pragma once

#include <iostream>
#include <chrono>
#include <iomanip>
#include <mutex>
#include <vector>
#include <memory>

#include "orpheus/Exception.hpp"

namespace Orpheus {
    class Loggable {
    private:
        std::vector<std::string> m_scopes;

    public:
        void addScope(const std::string& scope) {
            m_scopes.push_back(scope);
        }

        const std::vector<std::string>& getScopes() const {
            return m_scopes;
        }
    };

    class Log {
    private:
        class StreamWrapper {
        private:
            std::mutex& m_mutex;
            std::ostream& m_stream;

        public:
            StreamWrapper(std::mutex& mutex, std::ostream& stream) : m_mutex(mutex), m_stream(stream) {
                m_mutex.lock();
                auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
                auto tm = *std::gmtime(&tt);
                m_stream << std::put_time(&tm, "%Y-%m-%d %H:%M:%S ");
            }

            ~StreamWrapper() {
                m_stream << std::endl;
                m_mutex.unlock();
            }

            template<class T>
            StreamWrapper& operator<<(const T& value) {
                m_stream << value;
                return *this;
            }
        };

        std::mutex m_mutex;
        std::ostream& m_stream;

        Log(std::ostream& stream) : m_stream(stream) {}

        static Log& instance() {
            static Log log(std::cout);
            return log;
        }

        static void printScope(StreamWrapper& wrapper, const Loggable& loggable) {
            bool first = true;
            for (const auto& scope : loggable.getScopes()) {
                if (first) {
                    first = false;
                } else {
                    wrapper << "::";
                }
                wrapper << scope;
            }
            wrapper << ": ";
        }

        static void printScope(StreamWrapper& wrapper, std::shared_ptr<Loggable> loggable) {
            printScope(wrapper, *loggable.get());
        }

        static void printScope(StreamWrapper& wrapper, const Loggable* loggable) {
            printScope(wrapper, *loggable);
        }

        static void printScope(StreamWrapper& wrapper, const Exception& exception) {
            printScope(wrapper, *exception.getLoggable().get());
            wrapper << exception.what();
        }

        static void printScope(StreamWrapper&) {
        }

        template<class... T>
        static StreamWrapper message(const std::string& level, const T&... loggable) {
            auto& self = instance();
            StreamWrapper stream(self.m_mutex, self.m_stream);
            stream << level << ": ";

            printScope(stream, loggable...);

            return stream;
        }

    public:
        template<class... T> static StreamWrapper info(const T&... loggable)  { return message(" INFO", loggable...); }
        template<class... T> static StreamWrapper warn(const T&... loggable)  { return message(" WARN", loggable...); }
        template<class... T> static StreamWrapper fatal(const T&... loggable) { return message("FATAL", loggable...); }
        template<class... T> static StreamWrapper debug(const T&... loggable) { return message("DEBUG", loggable...); }
    };
}
