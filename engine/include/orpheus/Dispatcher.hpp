#pragma once

#include <unordered_map>
#include <functional>
#include <typeindex>

namespace Orpheus {
    template<class Key, class... Values>
    class Dispatcher {
    private:
        std::unordered_map<Key, std::function<void(Values...)>> m_table;

    public:
        template<class F>
        void registerKey(const Key& key, F&& router) {
            m_table[key] = router;
        }

        template<class F>
        bool dispatch(const Key& key, F&& router) {
            auto it = m_table.find(key);
            if (it != m_table.end()) {
                router(it->second);
                return true;
            }
            return false;
        }

        bool dispatch(const Key& key) {
            return dispatch(key, [](auto& router) { router(); });
        }
    };

    class TypeDispatcher {
    private:
        Dispatcher<std::type_index, void*> m_dispatcher;

    public:
        template<class T, class F>
        void registerType(F&& router) {
            m_dispatcher.registerKey(std::type_index(typeid(T)), [router](void* x) {
                router(*static_cast<typename std::remove_reference<T>::type*>(x));
            });
        }

        template<class T>
        bool dispatch(T& value) {
            return m_dispatcher.dispatch(std::type_index(typeid(T)), [&value](auto& router) {
                router(const_cast<typename std::remove_cv<T>::type*>(&value));
            });
        }
    };
}
