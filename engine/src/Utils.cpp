#include "orpheus/Utils.hpp"
#include <fstream>

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

std::vector<char> Orpheus::Utils::readFile(const std::string& path) {
    std::ifstream input(path, std::ios::binary | std::ios::ate);
    if (!input) {
        throw std::runtime_error("File '" + path + "' not found");
    }

    std::vector<char> res(input.tellg());
    input.seekg(0, std::ios::beg);
    input.read(res.data(), res.size());
    input.close();
    return res;
}

std::string Orpheus::Utils::readTextFile(const std::string& path) {
    auto vec = readFile(path);
    return std::string(vec.begin(), vec.end());
}
