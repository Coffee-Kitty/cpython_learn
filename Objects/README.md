# Objects/ 目录导读

这个目录包含 CPython 内建对象类型的实现。

旧的 `Objects/README` 仍然保留；这份文件提供了一张更细的阅读地图。

## 这里放了什么

在 CPython 里，每一个运行时值本质上都是一个 `PyObject *`。`Objects/` 实现的就是这些值背后的具体类型。

## 重要职责

- 内建标量类型（`int`、`bool`、`float`、`complex`）
- 容器类型（`list`、`tuple`、`dict`、`set`）
- 文本和二进制类型（`str`、`bytes`、`bytearray`）
- descriptor、method、iterator、slice、capsule 和 exception 支持
- 类型系统机制以及属性查找行为

## 关键文件

- `object.c` —— 基础对象辅助逻辑与单例对象定义
- `typeobject.c` —— `PyTypeObject`、类型创建、slot 解析以及 metatype 行为
- `dictobject.c` —— 字典实现
- `listobject.c` —— 列表实现
- `tupleobject.c` —— 元组实现
- `unicodeobject.c` —— Unicode 字符串实现
- `longobject.c` —— 任意精度整数实现
- `setobject.c` —— set / frozenset 实现
- `exceptions.c` —— 内建异常类型

## 为什么这个目录重要

理解 `Objects/`，就是在实践中理解 Python 的对象模型：

- 值在内存中如何布局
- 方法分发如何通过类型 slot 完成
- 容器行为和迭代如何实现
- `None`、`NotImplemented` 这类单例对象如何表示

## 这个目录如何连接到整棵源码树

- `Include/object.h` 和 `Include/cpython/object.h` 声明了这里使用的对象 / 类型结构
- `Python/ceval.c` 以及其他运行时组件会持续操作这些对象
- `Modules/` 会通过内建模块和扩展模块暴露其中很多类型

## 推荐阅读顺序

1. `object.c`
2. `typeobject.c`
3. 一个容器类型（`dictobject.c` 或 `listobject.c`）
4. 一个标量类型（`longobject.c` 或 `unicodeobject.c`）

## 这个目录回答哪些问题

- 运行时的 `PyObject` 到底是什么？
- `type` 作为 metatype 是怎样工作的？
- 内建容器如何存储自己的内容？
- Python 的特殊方法最终映射到哪些 C slot？
