#ifndef LITEACTORFRAMEWORK_LOCAL_ACTOR_HPP
#define LITEACTORFRAMEWORK_LOCAL_ACTOR_HPP
#include "actor/monitorable_actor.hpp"
#include "utils/intrusive_ptr.hpp"


namespace laf{
    // Mailbox
    class Mailbox {
        std::queue<Message> m_queue;
        mutable std::mutex m_mutex;
        //todo std::condition_variable cond_var_;
    public:
        Mailbox() = default;

        // remove copy constructor and assignment operator
        Mailbox(const Mailbox&) = delete;
        Mailbox& operator=(const Mailbox&) = delete;

        // remove move constructor and assignment operator
        Mailbox(Mailbox&&) = delete;
        Mailbox& operator=(Mailbox&&) = delete;

        void push(Message msg) {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_queue.push(std::move(msg));
        }

        bool empty() const {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_queue.empty();
        }

        Message pop() {
            std::lock_guard<std::mutex> lock(m_mutex);
            if (m_queue.empty()) {
                throw ActorError("Mailbox is empty");
            }
            Message msg = m_queue.front();
            m_queue.pop();
            return msg;
        }
    };

    // ActorContext
    // ActorContext 是 Actor 与外部世界交互的接口，提供了 Actor 运行时环境的关键组件。
    class ActorContext {
    private:
        ActorAddress m_self;            // 当前Actor地址
        ActorAddress m_current_sender;  // 当前消息发送者
        ActorSystem* m_system_;          // Actor系统引用

        template <typename T>
        ResponsePromise<T> make_response_promise() {
            // 创建响应Promise实现
            // ...
        }
    };


    // 本地Actor的核心实现
    /**
     * @brief 实现本地 Actor 的基础类
     * 消息处理机制：通过 become 和 unbecome 方法实现行为的动态切换；dequeue 从邮箱中取出消息并处理；支持保存和丢弃之前的行为
     * 生命周期管理：初始化 init 和清理 on_exit；quit 来退出；
     * todo 实现组通信
     */
    class LocalActor : public MonitorableActor {
    protected:
        std::string m_name; // Actor的名称（注意不是地址）
        Mailbox m_mailbox; // Actor的消息队列
        ActorContext m_context; // Actor的上下文


        LocalActor(ActorAddress addr, ActorSystem* system)
                : MonitorableActor(std::move(addr), system) {
            m_name = "DefaultActorName";
            m_mailbox = Mailbox();
            m_context = ; //todo
        }

    public:



        ActorContext& get_context() {
            return m_context;
        }

        // 在当前系统内创建另一个Actor
        ActorAddress spawn(); //todo


    };

    typedef intrusive_ptr<LocalActor> LocalActor_ptr;
}


#endif //LITEACTORFRAMEWORK_LOCAL_ACTOR_HPP
