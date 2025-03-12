# std::future notes

future 为了三五法则，删除了拷贝构造/赋值函数。

如果需要浅拷贝，实现共享同一个 future 对象，可以用 std::shared_future。

如果不需要返回值，std::async 里 lambda 的返回类型可以为 void， 这时 future 对象的类型为 std::future<void>。

同理有 std::promise<void>，他的 set_value() 不接受参数，仅仅作为同步用，不传递任何实际的值。

> 相关：[promise](../promise/README.md), [async](../async/README.md)
