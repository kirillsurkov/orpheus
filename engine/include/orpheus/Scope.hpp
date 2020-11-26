#pragma once

#include <vector>
#include <string>

namespace Orpheus {
    class Scoped {
    public:
        static std::vector<std::string> getScopes() {
            return std::vector<std::string>{};
        }
    };
}
