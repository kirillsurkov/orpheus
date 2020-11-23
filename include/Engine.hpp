#pragma once

#include <memory>

class Engine {
public:
    void loop();
};

using EnginePtr = std::shared_ptr<Engine>;
