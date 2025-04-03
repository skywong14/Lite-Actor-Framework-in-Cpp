#include <iostream>
#include "actor/behavior.hpp"
#include "utils/DSL/on.hpp"

using namespace laf;

struct exampleClass {
    int a = 54;
};

class my_actor {
    int counter_;
public:
    my_actor() : counter_(0) { }

    Behavior make_Behavior() {
        return Behavior{
                partial_function{
                        on<int>() >> [this](int delta) -> void {
                            counter_ += delta;
                            std::cout << "Counter is now " << counter_ << std::endl;
                        }
                },
                partial_function{
                        on<std::string>() >> [this](const std::string &msg) -> void {
                            std::cout << "Received message: " << msg << std::endl;
                        }
                },
                partial_function{
                        on<exampleClass>() >> [this](exampleClass tmp) -> void {
                            std::cout << "Received message: " << tmp.a << std::endl;
                        }
                }
        };
    }

    void test() {
        Behavior bh = make_Behavior();

        // 发送 int型消息
        any_single msg1(10); // type_ = typeid(int)
        bh(msg1); // => 匹配 int handler，输出"Counter is now 10"

        // 发送 string 型消息
        any_single msg2(std::string("Hello from test()!")); // type_ = typeid(std::string)
        bh(msg2); // => 匹配 string handler，输出"Received message: Hello from test()!"

        // 发送 double 型消息
        double d = 3.14;
        any_single msg3(d); // type_ = typeid(double)
        bool handled = bh(msg3); // => 不匹配 int / string，返回false
        if (!handled) {
            std::cout << "Double not handled!\n";
        }
        any_single msg4((exampleClass()));
        bh(msg4); // => 匹配 exampleClass handler，输出"Received message: 54"
    }

};

int main() {
    my_actor actor;
    actor.test();
    return 0;
}