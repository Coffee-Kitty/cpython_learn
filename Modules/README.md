# Modules/ 目录导读

这个目录包含 CPython 的内建模块、扩展模块，以及解释器主程序入口之后的支持代码。

旧的 `Modules/README` 仍然保留；这份文件会更详细地解释这个目录的结构。

## 这里放了什么

`Modules/` 是一个混合目录：

- 解释器的 CLI / bootstrap 主程序（`main.c`）
- 在 `config.c` 中注册的内建模块
- 用 C 实现的标准库模块
- 为动态加载扩展模块提供的支持代码

## 重要职责

- 承接 `Programs/python.c` 之后的进程启动路径
- 通过 `_PyImport_Inittab` 注册内建模块
- 实现性能敏感的标准库模块
- 提供 C 扩展模块入口，形式通常为 `PyMODINIT_FUNC PyInit_<name>()`

## 关键文件

- `main.c` —— `main()` 之后的解释器主控制流
- `config.c` —— 生成得到的内建模块表
- `gcmodule.c` —— 循环垃圾回收器实现，以及 `gc` 模块本体
- `_io/_iomodule.c` —— 核心 I/O 模块 bootstrap
- `_threadmodule.c` —— 线程支持
- `_sre.c` —— 正则引擎核心
- `posixmodule.c` / 平台对应实现 —— 操作系统绑定层

## 这个目录里的常见模式

### 内建模块注册

`config.c` 中包含 `_PyImport_Inittab[]`，把模块名映射到初始化函数。

### 扩展模块初始化

大多数 C 扩展模块都会暴露一个形如下面的入口：

```c
PyMODINIT_FUNC PyInit_modulename(void)
```

### 混合职责

这个目录里有些代码不是“可选模块”，而是运行时基础设施的一部分。`main.c` 和 `gcmodule.c` 就是典型例子。

## 这个目录如何连接到整棵源码树

- `Programs/python.c` 会进入 `Modules/main.c`
- `Python/import.c` 会从 `config.c` 查询内建模块注册信息
- `Objects/` 提供这些模块使用的具体对象和类型实现
- `Lib/` 提供构建在这些 C 模块之上的 Python 层标准库

## 推荐阅读顺序

1. `main.c`
2. `config.c`
3. `gcmodule.c`
4. 任选一个代表性的内建模块，例如 `_io/_iomodule.c` 或 `_threadmodule.c`

## 这个目录回答哪些问题

- `main()` 之后，解释器启动从哪里继续？
- 哪些模块总是被编进解释器？
- 一个 C 扩展模块是怎样注册自己的？
- 标准库里哪些部分为了性能或平台访问而用 C 实现？
