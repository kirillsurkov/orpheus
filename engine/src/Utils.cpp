#include "orpheus/Utils.hpp"

Orpheus::Utils::DeltaTime::DeltaTime() {
    m_last = std::chrono::system_clock::now();
}

float Orpheus::Utils::DeltaTime::getDelta() {
    auto now = std::chrono::system_clock::now();
    auto count = std::chrono::duration_cast<std::chrono::microseconds>(now - m_last).count();

    m_last = now;

    return count / 1000.0f;
}
