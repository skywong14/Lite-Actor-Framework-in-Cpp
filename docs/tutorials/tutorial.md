## LAF(Lite Actor Framework in C++) Tutorial

### 0. Introduction

当我在互联网上搜索时，发现没有好用的tutorial，那么就把我的学习过程整理成一份tutorial吧。

下面这个视频很清楚地讲解了Actor Model的基本概念：
[Hewitt, Meijer and Szyperski: The Actor Model (everything you wanted to know...)](https://www.youtube.com/watch?v=7erJ1DV_Tlo)

但这个视频所讨论的Actor Model是一个理论模型，而我们要实现的是一个具体的Actor Framework。
所以关于Actor Framework的实现，我找到了一个开源项目：[actor-framework](https://actor-framework.readthedocs.io/en/stable/)。
有位博主对其翻译并搬运在知乎专栏，链接为[link](https://www.zhihu.com/column/c_1624158969320116224)

以下的tutorial将会以这个开源项目为参考，但是会有一些不同。我们假设大家都理解了Actor Model的相关抽象概念，所以会重点关注它的实现。

### 1. Basic Actor Model

在这个阶段，我们的目标是完成一个能在本地系统上的，非并发的LAF，实现一些基本的功能。

虽然我们只是先基本功能，但为了后续的扩展，我们会设计一个比较完备的框架，虽然有些结构在这个阶段用不到。

#### 1.1 Requirements Analysis and Overall Design

为了高级功能的实现，针对Actor Model的基本特性，我们先定义一系列基本工具，它们包括：
- Message: 用于Actor之间的通信，由于ActorSystem的分布式特性，我们要求Message是平台无关且可序列化的。
- Error: 用于处理Actor的异常，内容包含错误码(Enum code)和信息(message context)。同样的，我们也要求Error是平台无关且可序列化的。
- Smart Pointers to Actors: 基于强弱引用计数的垃圾回收机制。
- Mailbox: 用于存储Actor的消息队列，一个支持并发的，多端写一端读的队列类型。

使用这些基本工具，我们需要以下模块，具体设计稍后讲详细介绍：
- Atom Actor: 用于实现Actor的基本功能，包括消息处理、生命周期管理等。
- Actor System: 用于管理部署在本地的Actor，之后也会负责与远程Actor System的通信


#### 1.2 Message, Error, and SmartPtr Design

##### 1.2.1 Message

基本要求：Inspectable, 有默认构造默认可构造, 拷贝可构造

我们要实现一个基类Message和派生出来的Default_Message。


actor应当至少能处理三种类别的消息：down_msg, exit_msg, error。这些是保证系统的稳定和容错的基本消息。

Actor通过Handler处理这些消息。详见1.3

##### 1.2.2 Error
具体而言，Error应该包含错误码(Enum code)和信息(message context)。

Constructor: (Enum code) or (Enum code, message context)

符合CAF标准的的SEC(System Error Code)规范，可在链接中查看：https://actor-framework.readthedocs.io/en/stable/core/Error.html

##### 1.2.3 SmartPtr


### 1.3 Actor Base Class and Interface Design

**注意**
- 一个actor可以有很多个address，也因此无法通过判断地址不同来推断actor不同
- axiom of actor model(Everything an actor can do): 1. create more actors 2. send more messages to actors it has addresses before. 3. designate how to handle the next message it receives

对于Actor，我们层次化的初步设计如下：
```
AbstractActor: addr, home_system
|
|--- MonitorableActor: attach, monitor, link_to
|       |--- ProxyActor: 远程actor的代理  
|       |
|       |--- LocalActor: name, response, make_response_promise, system(?), context, spawn, Mailbox, receive
|               |
|               |--- BlockingActor: 阻塞式 (同步), 专用线程
|               |
|               |--- EventBasedActor (Default): 非阻塞式 (异步事件驱动), 共享线程池
```

- spawn: 创建一个新的Actor
- receive: 按顺序迭代mailbox中的所有元素

> ProxyActor:
> 
> 代理模式：ProxyActor 作为本地节点上的代理，代表远程节点上的一个 actor。所有发送到该代理的消息都会被转发到实际的远程 actor。
> 
> 消息转发机制：ProxyActor 通过网络层将消息转发给远程 actor。
> 
> 消息处理与转发：首先检查消息是否为特殊的KILL_PROXY命令，如果是则执行清理操作。否则，将消息转发给远程actor
> 
> 链接管理：代理actor需要处理链接的创建和移除

**Handler**

Handler是Actor的消息处理器（由LocalActor管理），它是一个纯虚函数，由派生类实现。Handler的种类有：
- Down Handler

  actor有method:self->monitor(other)来监测其他actor的生命周期。当被监测者挂掉后会向监测者发送"down_msg"。

  使用self->set_down_handler(f)设置处理句柄，否则接收到的"down_msg"会被监测者丢掉。

  （这里的f是一种函数对象，它的形式是：void(down_msg&) 或者void(scheduled_actor*,down_msg&)。后者允许用户把一个函数作为处理down_msg的句柄。）
- Exit Handler

  使用"self->link_to(other)"把两个actor的生命绑在一起，他俩相互监视对方，不管哪个挂掉都会发送"exit_msg"。

  Default: actor在接收到一个"exit_msg"后，如果退出的原因不是"exit_reason::normal"，则actor会终止。这个机制用于在actor系统中传递错误状态。

  可以通过“set_exit_handler(f)”覆盖默认的句柄。
- Error Handler

  actor通过在消息句柄中返回一个"error"向其他actor发送error message。

  error message通常会让接收的actor终止，除非通过set_error_handler(f)定义了处理句柄。

- Default Handler

  如果一个actor的behavior不匹配输入消息，那default handler就会被调用。

  可以通过set_default_handler设置自定义的句柄，形式是result<message>(scheduled_actor*,message_view&)。

**Request**

处理一般消息

### 1.4 Actor System Design

- Initialization
- IdGenerator
- Registry（注册表）
- Scheduler（调度器） 
- Fault Tolerance（容错机制）
