#pragma once

#include <unordered_map>
#include <functional>

namespace Orpheus {
    class Dispatcher {
    private:
        std::unordered_map<std::size_t, std::function<void(void*)>> m_table;

    public:
        template<class T, class F>
        void registerType(F&& router) {
            m_table[typeid(T).hash_code()] = [router](void* x) {
                router(*static_cast<typename std::remove_reference<T>::type*>(x));
            };
        }

        template<class T>
        bool dispatch(T& t) {
            auto it = m_table.find(typeid(T).hash_code());
            if (it != m_table.end()) {
                it->second(const_cast<typename std::remove_cv<T>::type*>(&t));
                return true;
            }
            return false;
        }
    };
}
