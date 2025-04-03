#ifndef LITEACTORFRAMEWORK_ACTOR_ADDRESS_HPP
#define LITEACTORFRAMEWORK_ACTOR_ADDRESS_HPP

namespace laf{
    // Actor的地址，拥有ActorAddress即代表能够访问Actor
    // 包含所属的ActorSystem和System内部的Actor地址
    class ActorAddress {
    protected:
        // Actor的地址
        std::string m_id;
        // Actor所属的ActorSystem
        ActorSystem* m_system;

        ActorAddress(std::string id, ActorSystem* system)
                : m_id(std::move(id)), m_system(system) {}
    public:
        const std::string& get_id() const {
            return m_id;
        }

        ActorSystem* get_system() const {
            return m_system;
        }

        bool operator==(const ActorAddress& other) const {
            return m_address == other.m_address && m_system == other.m_system;
        }
    };
}

#endif //LITEACTORFRAMEWORK_ACTOR_ADDRESS_HPP
