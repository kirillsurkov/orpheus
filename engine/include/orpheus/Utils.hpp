#pragma once

#include <chrono>
#include <vector>

namespace Orpheus::Utils {
    class DeltaTime {
    private:
        std::chrono::time_point<std::chrono::system_clock> m_last;

    public:
        DeltaTime();
        ~DeltaTime() = default;

        float getDelta();
    };

    template<class T, class... Args>
    static std::vector<T> vectorAdd(const std::vector<T>& vec, Args&... args) {
        auto res = vec;
        (res.push_back(args), ...);
        return res;
    }
}
