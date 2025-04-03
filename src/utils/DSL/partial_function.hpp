#ifndef LITEACTORFRAMEWORK_PARTIAL_FUNCTION_HPP
#define LITEACTORFRAMEWORK_PARTIAL_FUNCTION_HPP

#pragma once
#include "utils/any_tuple.hpp"
#include <vector>
#include <functional>


namespace laf {
    /**
     * @brief 由 on<T>() >> lambda 生成的 handler，用于匹配输入消息的类型。
     *        operator(any_single)调用时，如果 any_single 格式正确，则执行指定的 lambda，并返回 true。
     */
    template <class T>
    class type_match_handler {
        std::function<void(T&)> func_;
    public:
        explicit type_match_handler(std::function<void(T&)> func) : func_(std::move(func)) {}

        bool operator()(any_single& msg) const {
            // if matched, call func_, return true
            if (msg.get_type() == typeid(T)) {
                T val = msg.get_as<T>();
                func_(val);
                return true;
            }
            // if not matched, return false
            return false;
        }
    };

    /**
     * @brief 允许存放多个传入 any_single 返回 bool 的 Handler(用 type_match_handler 实现) 用于匹配输入匹配函数，并在调用时依次尝试。只要有一个匹配成功，则返回 true。
     */
    class partial_function {
        std::vector<std::function<bool(any_single&)>> handlers_;
    public:
        partial_function() = default;

        // 可以用 std::function<bool(any_tuple&)> 来初始化
        explicit partial_function(std::function<bool(any_single&)> handler) {
            if (handler)
                handlers_.push_back(std::move(handler));
        }

        partial_function(const partial_function&) = default;
        partial_function(partial_function&&) = default;
        partial_function& operator=(const partial_function&) = default;
        partial_function& operator=(partial_function&&) = default;

        // or_else
        [[nodiscard]] partial_function or_else(const partial_function& other) const {
            partial_function result(*this);
            for (const auto& h : other.handlers_) {
                result.handlers_.push_back(h);
            }
            return result;
        }

        // 在调用时依次尝试每个 handler，只要有一个返回true即表示处理成功
        bool operator()(any_single& value) const {
            for (auto& h : handlers_)
                if (h(value)) {
                    return true;
                }
            return false;
        }

        bool operator()(const any_single& value) const {
            any_single copy(value);
            return (*this)(copy);
        }

        bool operator()(any_single&& value) const {
            return (*this)(value);
        }

        [[nodiscard]] bool undefined() const {
            return handlers_.empty();
        }
    };

    // 重载逗号运算符（但不好用，逗号运算符优先级太低）
    partial_function operator,(const partial_function& lhs, const partial_function& rhs) {
        return lhs.or_else(rhs);
    }

} // namespace cppa

#endif //LITEACTORFRAMEWORK_PARTIAL_FUNCTION_HPP
