#ifndef LITEACTORFRAMEWORK_DURATION_HPP
#define LITEACTORFRAMEWORK_DURATION_HPP

#pragma once


namespace laf {
    enum class time_unit : uint32_t {
        none = 0,
        microseconds = 1,
        milliseconds = 1000,
        seconds = 1000000
    };

    class Duration {
    public:
        time_unit unit;
        uint32_t  count;

        // 单位不为 none 认为有效
        bool valid() const {
            return (unit != time_unit::none);
        }

        bool is_zero() const {
            return !count;
        }

        Duration() : unit(time_unit::none), count(0) {}
        Duration(time_unit u, uint32_t v) : unit(u), count(v) {}
    };
}


#endif //LITEACTORFRAMEWORK_DURATION_HPP
