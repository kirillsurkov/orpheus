#pragma once

#include <stdexcept>
#include <memory>
#include <vector>

namespace Orpheus {
    class Loggable;

    class Exception : public std::runtime_error {
    private:
        std::vector<std::string> m_scopes;
        std::unique_ptr<Loggable> m_loggable;

    public:
        Exception(const Loggable* loggable, const std::string& message);
        const std::unique_ptr<Loggable>& getLoggable() const;
    };
}
