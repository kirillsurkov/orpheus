#include "orpheus/Utils.hpp"

Orpheus::Utils::StopWatch::StopWatch() :
    m_last(std::chrono::system_clock::now())
{
}

float Orpheus::Utils::StopWatch::split() {
    auto now = std::chrono::system_clock::now();
    auto count = std::chrono::duration_cast<std::chrono::microseconds>(now - m_last).count();

    m_last = now;

    return 1.0f * count / 1e6;
}
