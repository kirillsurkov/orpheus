#pragma once

#include <unordered_map>
#include <functional>
#include <typeindex>
#include <memory>

#include "orpheus/Utils.hpp"

namespace Orpheus {
    template<class Key, class... Values>
    class Dispatcher {
    private:
        std::unordered_map<Key, std::function<void(Values...)>> m_table;

    public:
        Dispatcher() {
        }

        Dispatcher(Dispatcher&& dispatcher) {
            *this = std::move(dispatcher);
        }

        Dispatcher(const Dispatcher& dispatcher) {
            *this = dispatcher;
        }

        Dispatcher& operator=(Dispatcher&& dispatcher) {
            m_table = std::move(dispatcher.m_table);
            return *this;
        }

        Dispatcher& operator=(const Dispatcher& dispatcher) {
            m_table = dispatcher.m_table;
            return *this;
        }

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

        void clear() {
            m_table.clear();
        }
    };

    class TypeDispatcher {
    private:
        Dispatcher<std::type_index, void*> m_dispatcher;

    public:
        TypeDispatcher() {
        }

        TypeDispatcher(TypeDispatcher&& dispatcher) {
            *this = std::move(dispatcher);
        }

        TypeDispatcher(const TypeDispatcher& dispatcher) {
            *this = dispatcher;
        }

        TypeDispatcher& operator=(TypeDispatcher&& dispatcher) {
            m_dispatcher = std::move(dispatcher.m_dispatcher);
            return *this;
        }

        TypeDispatcher& operator=(const TypeDispatcher& dispatcher) {
            m_dispatcher = dispatcher.m_dispatcher;
            return *this;
        }

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

        void clear() {
            m_dispatcher.clear();
        }
    };

    class CommandDispatcher {
    private:
        TypeDispatcher m_dispatcher;

    public:
        CommandDispatcher() {
        }

        CommandDispatcher(CommandDispatcher&& dispatcher) {
            *this = std::move(dispatcher);
        }

        CommandDispatcher(const CommandDispatcher& dispatcher) {
            *this = dispatcher;
        }

        CommandDispatcher& operator=(CommandDispatcher&& dispatcher) {
            m_dispatcher = std::move(dispatcher.m_dispatcher);
            return *this;
        }

        CommandDispatcher& operator=(const CommandDispatcher& dispatcher) {
            m_dispatcher = dispatcher.m_dispatcher;
            return *this;
        }

        template<class T, class U>
        void registerCommand(U&& receiver) {
            auto* receiverPtr = Utils::ptr(std::forward<U>(receiver));
            m_dispatcher.registerType<const std::shared_ptr<T>&>([receiverPtr](const std::shared_ptr<T>& command) { receiverPtr->postCommand(command); });
        }

        template<class T>
        bool dispatch(T&& value) {
            return m_dispatcher.dispatch(value);
        }

        void clear() {
            m_dispatcher.clear();
        }
    };
}
