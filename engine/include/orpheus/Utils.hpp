#pragma once

#include <chrono>
#include <vector>
#include <array>

namespace Orpheus::Utils {
    namespace Concat {
        template<std::size_t N, class ArrT, const std::array<ArrT, N>& Arr>
        class Array {
        private:
            template<std::size_t S, const std::array<char, S>& Chars, class...>
            struct ToChars;

            template<std::size_t S, const std::array<char, S>& Chars, std::size_t... I>
            struct ToChars<S, Chars, std::index_sequence<I...>> {
                static constexpr const char value[]{Chars[I]..., 0};
            };

            static constexpr std::size_t getTotalSize() {
                std::size_t res = 0;
                for (const auto& str : Arr) {
                    res += std::string_view(str).size();
                }
                return res;
            }

            static constexpr auto toArray() {
                constexpr std::string_view delimiter = "::";
                constexpr std::size_t size = getTotalSize();
                std::array<char, size + 1 + Arr.size() * delimiter.size()> arr{};
                int i = 0;
                unsigned int n = 0;
                for (const auto& chars : Arr) {
                    std::string_view str(chars);
                    for (std::size_t j = 0; j < str.size(); j++) {
                        arr[i++] = str[j];
                    }
                    if (++n < Arr.size() && str.size() > 0) {
                        for (const auto& d : delimiter) {
                            arr[i++] = d;
                        }
                    }
                }
                return arr;
            }

            static constexpr auto arr = toArray();

        public:
            static constexpr std::string_view value = ToChars<arr.size(), arr, std::make_index_sequence<arr.size()>>::value;
        };

        template<const std::string_view&... Strs>
        struct ToArray {
            static constexpr auto value = std::array{Strs..., };
        };

        template<const std::string_view&... Strs>
        struct Strings : public Array<sizeof...(Strs), std::string_view, ToArray<Strs...>::value> {
        };
    }

    class DeltaTime {
    private:
        std::chrono::time_point<std::chrono::system_clock> m_last;

    public:
        DeltaTime();
        ~DeltaTime() = default;

        float getDelta();
    };
}
