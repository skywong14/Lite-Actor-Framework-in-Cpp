#ifndef LITEACTORFRAMEWORK_BEHAVIOR_HPP
#define LITEACTORFRAMEWORK_BEHAVIOR_HPP

#pragma once
#include "utils/DSL/on.hpp"
#include "utils/DSL/partial_function.hpp"

// todo 优化声明，使得 Behavior 可以直接使用 on<T>() >> lambda 的语法（去掉 partial_function 的中间变量）

namespace laf {
    /**
     * @brief Behavior 类用于描述 Actor Model 中 actor 的行为
     * Behavior 是对 partial_function 的包装
     * partial_function 可以拼接，用 std::vector 管理了多个 handler
     * something like this is allowed:
     * Behavior b {
            partial_function {
                on<int>() >> [this](int x) {  codes  }
            },
            partial_function {
                on<std::string>() >> [this](std::string x) {  codes  }
            },
            ...
            partial_function {
                on<MyClass>() >> [this](MyClass x) {  codes  }
            }
            };
     * and after create a Behavior class, we can use something like "b(1), b("hello"), b(MyClass())"
     */

    class Behavior {
        partial_function pf_;

        // 递归终止条件
        partial_function combine_pfs(partial_function pf) {
            return pf;
        }

        // 递归组合函数
        template<typename... Functions>
        partial_function combine_pfs(partial_function first, Functions... rest) {
            return first.or_else(combine_pfs(std::move(rest)...));
        }
    public:
        // ----------constructors----------

        // single partial_function
        Behavior(partial_function pf) : pf_(std::move(pf)) {}

        // multiple partial_function
        template<typename... pfs>
        Behavior(partial_function first, pfs... rest) {
            pf_ = combine_pfs(std::move(first), std::move(rest)...);
        }

        Behavior(const Behavior&) = default;
        Behavior(Behavior&&) = default;
        Behavior& operator=(const Behavior&) = default;
        Behavior& operator=(Behavior&&) = default;

        // ----------operator()----------

        bool operator()(const any_single& msg) {
            if (!pf_.undefined()) {
                return pf_(msg);
            }
            return false;
        }

        bool operator()(any_single&& msg) {
            if (!pf_.undefined()) {
                return pf_(std::move(msg));
            }
            return false;
        }

        // ----------methods----------
        bool undefined() const {
            bool no_pf = pf_.undefined();
            return no_pf;
        }
    };
}

#endif //LITEACTORFRAMEWORK_BEHAVIOR_HPP
