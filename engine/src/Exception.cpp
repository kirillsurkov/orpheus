#include "orpheus/Exception.hpp"
#include "orpheus/Log.hpp"

Orpheus::Exception::Exception(const Loggable* loggable, const std::string& message) :
    std::runtime_error(message),
    m_loggable(std::make_unique<Loggable>(*loggable))
{
}

const std::unique_ptr<Orpheus::Loggable>& Orpheus::Exception::getLoggable() const {
    return m_loggable;
}
