#ifndef LITEACTORFRAMEWORK_MONITORABLE_ACTOR_HPP
#define LITEACTORFRAMEWORK_MONITORABLE_ACTOR_HPP
#include "actor/abstract_actor.hpp"
#include "actor/attachable.hpp"
#include "actor/local_actor.hpp"

namespace laf {
    // todo 特殊处理远程节点
    /**
     * @brief Implements linking and monitoring for actors.
     * link: 支持Actor之间的链接关系 （当一个actor退出时，它会向所有链接的actor发送EXIT消息；如果A链接到B，那么B也链接到A）
     * monitor: 提供Actor生命周期监控机制
     */
    // 提供Actor生命周期监控机制
    // 支持Actor之间的链接关系
    class MonitorableActor : public AbstractActor {
        std::vector<ActorAddress> monitored_actors_;  // 被监控的Actors
        std::vector<ActorAddress> linked_actors_;     // 被链接的Actors
        std::vector<Attachable_ptr> attachables_;     // 附加的可监控对象
        std::uint32_t exit_reason_;                   // Actor退出原因
    protected:
        MonitorableActor(ActorAddress addr, ActorSystem* system)
                : AbstractActor(std::move(addr), system), exit_reason_(0) {}
    public:
        // -----------attach & monitor-----------

        /**
         * @brief 一个 actor 通过附加一个 attachable 对象到被监控的 actor 来实现 monitor
         * 当目标 actor 退出时，会遍历所有附加的 attachable 对象并调用它们的 actor_exited 方法
         * 监控者可以通过 detach() 方法和一个特定的token移除监控
         */
        // 附加一个可监控对象到Actor
        void attach(Attachable_ptr ptr) {

        }

        // 按条件移除指定标记的可监控对象  `
        void detach(const token& what) {

        }

        // monitor(other)来监测其他actor的生命周期。当被监测者挂掉后会向监测者发送"down_msg"。
        bool monitor(LocalActor_ptr& other) {

        }

        // -----------link-----------
        /*
         * link_to_local(actor&) - 建立到另一个本地 actor 的链接,他俩相互监视对方，不管哪个挂掉都会发送"exit_msg"
         * unlink_from_local(actor&) - 移除与另一个本地 actor 的链接
         */
        bool link_to_local(intrusive_ptr<LocalActor>& other) {

        }

        bool unlink_from_local(intrusive_ptr<LocalActor>& other) {

        }

        bool link_to() {

        }

        bool unlink_from() {

        }
    };

    typedef intrusive_ptr<MonitorableActor> MonitorableActor_ptr;
}

#endif //LITEACTORFRAMEWORK_MONITORABLE_ACTOR_HPP
