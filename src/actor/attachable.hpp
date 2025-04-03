#ifndef LITEACTORFRAMEWORK_ATTACHABLE_HPP
#define LITEACTORFRAMEWORK_ATTACHABLE_HPP

#pragma once
#include <memory>

namespace laf {

    struct token {
        const std::type_info& subtype;
        const void* ptr;
    };

    /**
     * @brief 在 actor 退出时执行特定操作，是 Monitoring 功能的核心组件。
     * 当一个 actor 想要监控另一个 actor 时，会创建一个 attachable 对象并附加到目标 actor 上:
     * 监控者 actor 会创建一个 attachable 的子类实例，重写 actor_exited 方法以在目标 actor 退出时执行自定义逻辑
     * 将这个 attachable 实例通过 attach() 方法附加到目标 actor 上
     * 当目标 actor 退出时，会遍历所有附加的 attachable 对象并调用它们的 actor_exited 方法
     * 监控者可以通过 detach()方法和一个特定的 token 移除监控
     */
    class Attachable {
        Attachable(const Attachable&) = delete;
        Attachable& operator=(const Attachable&) = delete;
    protected:
        Attachable() = default;

    public:
        virtual ~Attachable();

        /**
         * @brief Executed if the actor finished execution with given @p reason.
         * 当被监控的 actor 退出时，会调用这个方法
         */
        virtual void actor_exited(std::uint32_t reason) = 0;

        /**
         * @brief Selects a group of @c attachable instances by @p what.
         * 用于确定一个特定的 token 是否"匹配"这个 attachable
         * 这使得可以在不知道具体 attachable 类型的情况下选择性地移除 attachable
         */
        virtual bool matches(const token& what) = 0;
    };

    typedef std::unique_ptr<Attachable> Attachable_ptr;
}


#endif //LITEACTORFRAMEWORK_ATTACHABLE_HPP
