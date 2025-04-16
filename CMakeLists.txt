cmake_minimum_required(VERSION 3.22)
project(LiteActorFramework)

set(CMAKE_CXX_STANDARD 20)

# 添加头文件路径
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

# 添加库文件
add_library(LiteActorFramework STATIC
        # actor
        src/actor/abstract_actor.hpp
        src/actor/actor_address.hpp
        src/actor/attachable.hpp
        src/actor/behavior.hpp
        src/actor/blocking_actor.hpp
        src/actor/eventbased_actor.hpp
        src/actor/exit_monitor.hpp
        src/actor/local_actor.hpp
        src/actor/monitorable_actor.hpp

        # system
        src/system/actor_system.hpp
        src/system/actor_system_config.hpp
        src/system/id_generator.hpp
        src/system/scheduler.hpp

        # utils
        src/utils/DSL/on.hpp
        src/utils/DSL/partial_function.hpp
        src/utils/any_tuple.hpp
        src/utils/intrusive_ptr.hpp
        src/utils/laf_error.hpp
        src/utils/message.hpp
        src/utils/utils.hpp
        src/utils/duration.hpp
)

set_target_properties(LiteActorFramework PROPERTIES LINKER_LANGUAGE CXX)

add_executable(behavior_test testbench/behavior_testbench.cpp)

add_executable(hello_world testbench/hello_world.cpp)
