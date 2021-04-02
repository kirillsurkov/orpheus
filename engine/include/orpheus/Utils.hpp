#pragma once

#include <chrono>
#include <vector>
#include <string>

namespace Orpheus::Utils {
    template<class T>
    class TypeIdentity {
        using Type = T;
    };

    template<class T>
    T* ptr(T* obj) {
        return obj;
    }

    template<class T>
    T* ptr(T& obj) {
        return &obj;
    }

    class StopWatch {
    private:
        std::chrono::time_point<std::chrono::system_clock> m_last;

    public:
        StopWatch();
        ~StopWatch() = default;

        float split();
    };

    std::vector<char> readFile(const std::string& path);
    std::string readTextFile(const std::string& path);
}
