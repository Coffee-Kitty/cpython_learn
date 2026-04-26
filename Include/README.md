# Include/ 目录导读

这个目录包含 CPython 的核心头文件。

## 这里放了什么

`Include/` 定义了解释器在 C 语言层面暴露出来的结构：

- 公共的 Python/C API 头文件
- 核心对象 / 类型声明
- 特性开关和宏定义
- `cpython/` 子目录下的部分 CPython 专用头文件

## 重要职责

- 定义 `PyObject`、`PyVarObject`、引用计数宏以及常见 API
- 声明 `PyTypeObject` 及相关 slot / 方法表结构
- 暴露模块、导入、内存、Unicode、数字、序列和运行时接口

## 关键文件

- `Python.h` —— 公共 C API 的总入口头文件
- `object.h` —— 基础对象模型与引用计数辅助逻辑
- `cpython/object.h` —— CPython 专用的对象 / 类型布局细节
- `import.h` / `cpython/import.h` —— 与导入相关的声明
- `unicodeobject.h` —— Unicode API 与结构定义
- `dictobject.h`、`listobject.h`、`tupleobject.h` —— 具体对象类型 API

## 应该怎样读这个目录

如果你正在沿着源码追踪运行时行为，就应该借助这里的头文件去理解那些 `.c` 文件正在操作的数据结构和宏。

一个不错的起步顺序是：

1. `Python.h`
2. `object.h`
3. `cpython/object.h`
4. 任选一个具体类型头文件，例如 `dictobject.h`

## 这个目录如何连接到整棵源码树

- `Python/` 会包含这里的头文件来实现运行时逻辑
- `Objects/` 会用它们来实现具体类型
- `Modules/` 会用它们来实现内建模块和扩展模块
- 外部 C 扩展也会从这里包含公共头文件，以嵌入或扩展 Python

## 这个目录回答哪些问题

- Python 对象在 C 里长什么样？
- 哪些宏负责管理引用计数？
- 类型 slot 是怎样声明的？
- 哪些 API 是公开的，哪些是 CPython 专用的？
