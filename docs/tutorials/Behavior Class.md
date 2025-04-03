## Tutorial for Behavior Class

*本篇教程将介绍 Actor Model 中的 Behavior 类的实现。*

Behavior 类用于描述 Actor Model 中 actor 的行为，通过封装一个 partial_function 来实现。

相关代码结构：
```
src/
├── actor/
│   └── Behavior.hpp               # 对 partial_function.hpp 的封装
└── utils/
    ├── any_tuple.hpp              # 目前实现了 any_single， 支持单个任意类型的参数
    └── DSL/
        ├── on.hpp                 # 实现 on<T>() >> labmda
        └── partial_function.hpp   # 存储若干 Handler

benchmark/
└── behavior_benchmark.cpp    # 示例代码
```

我们希望实以下的效果
```cpp
// 声明一个有三种类型的行为的 Behavior
Behavior b {
    partial_function {
        on<int>() >> [this](int x) {  codes  }
    },
    partial_function {
        on<std::string>() >> [this](std::string x) {  codes  }
    },
    partial_function {
        on<MyClass>() >> [this](MyClass x) {  codes  }
    }
};
// 之后可以直接使用"b(1), b("hello"), b(MyClass())"来调用
```

这样将 Behavior 可以对不同类型的输入做出自定义的反应，且由于传入了 this 指针，Behavior 可以直接访问 Actor 的成员变量和方法。

具体原理：
我们观察到 partial_function 介绍到了一个格式如 "on<T>() >> lambda" 的函数对象。 而这个 Domain-Specific Language(DSL) 的语法，我们实际上通过 template<T> on() 与 重载的 operator>> 来实现。

on<T>() 这个函数创建了一个 on_type_builder<T> 的实例， 随后该实例调用 operator>> 来保存后面的 lambda 表达式。

然后，我们实现了
```cpp
return partial_function(
                    [tmh = type_match_handler<T>(handler)](any_single& msg) -> bool {
                        return tmh(msg);
                    }
            );
```
这个函数的作用是将传入的 lambda 表达式转换为一个 type_match_handler 后传入 partial_function 对象。

partial_function 使用 std::vector 存储了一些列 bool(any_single&) 的函数对象（此处均为 type_match_handler），通过 operator() 来调用这些函数对象。

partial_function 的 operator() 会遍历这些函数对象，直到找到一个返回 true 的函数对象，并将其传入的参数传递给 lambda 表达式。 这样就实现了对不同类型的输入做出不同的反应。

同时 partial_function 支持用逗号拼接，但由于逗号运算的优先级太低，实际效果不好。