#ifndef LITEACTORFRAMEWORK_ABSTRACT_ACTOR_HPP
#define LITEACTORFRAMEWORK_ABSTRACT_ACTOR_HPP

#include "system/actor_system.hpp"
#include "actor/actor_address.hpp"
#include "utils/intrusive_ptr.hpp"

#include "utils/laf_error.hpp"
#include <vector>

namespace laf {
    // 管理Actor的地址和所属系统
    class AbstractActor {
    protected:
        ActorAddress m_default_address;
        std::vector<ActorAddress> m_other_address;
        Actors_system* m_system;

        AbstractActor(ActorAddress addr, ActorSystem* system)
            : m_default_address(addr), m_system(system), m_other_address() {
            // todo register in system
        }
    public:
        Actors_system& get_homeSystem() {
            return *m_system;
        }
        ActorAddress& get_default_address() {
            return m_default_address;
        }

        // 创建一个新地址
        ActorAddress& create_address() {
            ActorAddress addr = m_default_address.get_id() + std::to_string(m_other_address.size());
            m_other_address.push_back(addr);
            // todo register in system
            return m_other_address.back();
        }
    };

    typedef intrusive_ptr<AbstractActor> AbstractActor_ptr;
}

#endif //LITEACTORFRAMEWORK_ABSTRACT_ACTOR_HPP
