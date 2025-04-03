#ifndef LITEACTORFRAMEWORK_EXIT_MONITOR_HPP
#define LITEACTORFRAMEWORK_EXIT_MONITOR_HPP

namespace laf {

    class ExitMonitor : public Attachable {
        actor_ptr m_observer; // 观察者
        actor_ptr m_observed; // 被观察者
    public:
        // todo override actor_exited and matches
    };
}

#endif //LITEACTORFRAMEWORK_EXIT_MONITOR_HPP
