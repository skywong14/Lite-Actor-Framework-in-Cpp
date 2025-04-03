#ifndef LITEACTORFRAMEWORK_ON_HPP
#define LITEACTORFRAMEWORK_ON_HPP

#pragma once
#include <functional>
#include <typeinfo>
#include <typeindex>
#include <memory>
#include "utils/DSL/partial_function.hpp"

namespace laf {

    /**
     * @brief Such a DSL enables syntax like `on<int>() >> lambda`.
     *        At this stage, we will first implement a basic "on_type".
     */
    template <class T>
    class on_type_builder {
    private:
        std::function<void(T&)> handler;
    public:
        // 用 >> 将 lambda 收集到 handler 中
        on_type_builder<T> operator>>(std::function<void(T&)> fun) {
            handler = std::move(fun);
            return *this;
        }

        // convert to partial_function, allow implicit conversion
        explicit operator partial_function() const {
            // 用 type_match_handler<T> 来生成一个 bool(any_tuple&) 的回调
            return partial_function(
                    [tmh = type_match_handler<T>(handler)](any_single& msg) -> bool {
                        return tmh(msg);
                    }
            );
        }
    };

    // entry of DSL: on<type>()
    template <class T>
    on_type_builder<T> on() {
        return on_type_builder<T>{};
    }
}

#endif //LITEACTORFRAMEWORK_ON_HPP
