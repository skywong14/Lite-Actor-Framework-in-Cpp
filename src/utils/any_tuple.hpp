#ifndef LITEACTORFRAMEWORK_ANY_TUPLE_HPP
#define LITEACTORFRAMEWORK_ANY_TUPLE_HPP

#pragma once
#include <vector>
#include <memory>
#include <typeinfo>
#include <stdexcept>
#include <utility>
#include <type_traits>

namespace laf {

    class any_single {

        class element_base {
        public:
            virtual const std::type_info& type() const = 0;  // 返回该元素的类型信息
            virtual std::unique_ptr<element_base> clone() const = 0; // 克隆该元素
            // virtual bool type_equals(const element_base* other) const = 0; // 判断两个元素类型是否相等
        };

        // 具体元素实现
        template<typename T>
        class element_impl : public element_base {
            T value_;
        public:
            template<typename V>
            explicit element_impl(V&& value) : value_(std::forward<V>(value)) {}

            virtual const std::type_info& type() const {
                return typeid(T);
            }

            virtual std::unique_ptr<element_base> clone() const {
                return std::make_unique<element_impl<T>>(value_);
            }

            const T& get() const {
                return value_;
            }

            ~element_impl() = default;
        };

    private:
        std::unique_ptr<element_base> element;
    public:
        // constructors
        any_single() = default;

        // ATTENTION: T&& 是一个 universal reference，可以接受左值也可以接受右值，因为T是一个模板参数
        template<class T>
        explicit any_single(T&& val) :
                element(std::make_unique<element_impl<std::decay_t<T>>>(std::forward<T>(val))) {}

        any_single(const any_single& other) noexcept {
            if (other.element)
                element = other.element->clone();
            else
                element = nullptr;
        }

        any_single(any_single&& other) noexcept {
            element = std::move(other.element);
        }

        // methods
        const std::type_info& get_type() const {
            if (!element) throw std::bad_cast();
            return element->type();
        }

        template<typename T>
        const T& get_as() {
            if (!element || element->type() != typeid(T))
                throw std::bad_cast();
            return dynamic_cast<element_impl<T>*>(element.get())->get();
        }

        template<typename T>
        T& get_as_mutable() {
            if (!element || element->type() != typeid(T))
                throw std::bad_cast();
            return dynamic_cast<element_impl<T>*>(element.get())->get();
        }

        bool type_equals(const std::type_info& type) const {
            if (!element) throw std::bad_cast();
            return element->type() == type;
        }

        bool type_equals(const any_single& other) const {
            if (!element || !other.element) throw std::bad_cast();
            return element->type() == other.element->type();
        }
    };


    /*
    class any_tuple {
        // 单个元素
        class element_base {
        public:
            virtual ~element_base() = 0;
            virtual const std::type_info& type() const = 0;  // 返回该元素的类型信息
            virtual std::unique_ptr<element_base> clone() const = 0; // 克隆该元素
            virtual bool equals(const element_base* other) const = 0; // 判断两个元素是否相等
        };

        // 具体元素实现
        template<typename T>
        class element_impl : public element_base {
            T value_;
        public:
            element_impl(const T& value) : value_(value) {}
            element_impl(T&& value) : value_(std::move(value)) {}

            virtual const std::type_info& type() const {
                return typeid(T);
            }

            virtual std::unique_ptr<element_base> clone() const {
                return std::make_unique<element_impl<T>>(value_);
            }

            virtual bool equals(const element_base* other) const {
                if (type() != other->type())
                    return false;
                const element_impl<T>* other_impl = dynamic_cast<const element_impl<T>*>(other);
                if (!other_impl) throw std::bad_cast(); // 确保类型匹配
                return value_ == other_impl->value_; // 使用 operator== 比较
            }

            const T& get() const {
                return value_;
            }

            ~element_impl() override = default;
        };
    private:
        std::vector<std::unique_ptr<element_base>> elements;

        template<typename First, typename... Res>
        void add_elements(First&& first, Res&&... res) {
            using T = typename std::decay<First>::type; // get plain T, remove const/volatile and reference
            elements.push_back(std::make_unique<element_impl<T>>(std::forward<First>(first)));
            add_elements(std::forward<Res>(res)...);
        }

    public:
        any_tuple() = default;

        template<typename... Args>
        any_tuple(Args&&... args) {
            add_elements(std::forward<Args>(args)...); // add all
        }

        any_tuple(const any_tuple& other) {
            for (const auto& elem : other.elements) {
                elements.push_back(elem->clone());
            }
        }

        any_tuple(any_tuple&& other) {
            elements = std::move(other.elements);
        }

        any_tuple& operator=(const any_tuple& other) {
            if (&other == this) return *this;
            elements.clear();
            for (const auto& elem : other.elements) {
                elements.push_back(elem->clone());
            }
            return *this;
        }

        any_tuple& operator=(any_tuple&& other) {
            if (&other == this) return *this;
            elements = std::move(other.elements);
            return *this;
        }

        size_t size() const {
            return elements.size();
        }

        template<typename T>
        const T& get_as(size_t index) {
            if (index >= elements.size())
                throw std::out_of_range("Index out of range");
            if (elements[index]->type() != typeid(T))
                throw std::bad_cast();
            return dynamic_cast<element_impl<T>*>(elements[index].get())->get();
        }

        template<typename T>
        T& get_as_mutable(size_t index) {
            if (index >= elements.size())
                throw std::out_of_range("Index out of range");
            if (elements[index]->type() != typeid(T))
                throw std::bad_cast();
            return dynamic_cast<element_impl<T>*>(elements[index].get())->get();
        }

        const std::type_info& get_type(size_t index) const {
            if (index >= elements.size())
                throw std::out_of_range("Index out of range");
            return elements[index]->type();
        }

        bool type_equals(size_t index, const std::type_info& type) const {
            if (index >= elements.size())
                throw std::out_of_range("Index out of range");
            return elements[index]->type() == type;
        }

        template<typename T>
        bool type_equals(size_t index) const {
            if (index >= elements.size())
                throw std::out_of_range("Index out of range");
            return elements[index]->type() == typeid(T);
        }

        bool operator==(const any_tuple& other) const {
            if (this == &other) return true;
            if (elements.size() != other.elements.size()) return false;
            for (size_t i = 0; i < elements.size(); i++)
                if (!elements[i]->equals(other.elements[i].get())) return false;
            return true;
        }

        bool operator!=(const any_tuple& other) const {
            return !(*this == other);
        }

        void clear() {
            elements.clear();
        }
    };

    template<typename... Args>
    any_tuple make_any_tuple(Args&&... args) {
        return any_tuple(std::forward<Args>(args)...);
    }
    */
}

#endif //LITEACTORFRAMEWORK_ANY_TUPLE_HPP
