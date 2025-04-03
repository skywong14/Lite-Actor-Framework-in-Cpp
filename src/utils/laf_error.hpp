#ifndef LITEACTORFRAMEWORK_LAF_ERROR_HPP
#define LITEACTORFRAMEWORK_LAF_ERROR_HPP

#include <exception>
#include <string>
#include <utility>

namespace laf {
    class ActorError : public std::exception {
    protected:
        std::string message_;
    public:
        explicit ActorError(std::string message) : message_(std::move(message)) {}
        const char* what() const noexcept override {
            return message_.c_str();
        }
    };

    class SystemError : public ActorError {
    public:
        explicit SystemError(const std::string& message)
                : ActorError("System error: " + message) {}
    };

    class TimeoutError : public ActorError {
    public:
        explicit TimeoutError(const std::string& message)
                : ActorError("Timeout error: " + message) {}
    };

}

#endif //LITEACTORFRAMEWORK_LAF_ERROR_HPP
