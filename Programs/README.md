# Programs/ 目录导读

这个目录包含 CPython 可执行程序对应的源码文件。

旧的 `Programs/README` 仍然保留；这份文件会更详细地说明“程序入口”这一层的作用。

## 这里放了什么

`Programs/` 是进程级入口层。它包含一些很小的可执行入口文件，以及参与解释器构建或运行的辅助程序。

## 关键文件

- `python.c` —— 进程级 `main()` 入口，从这里跳入解释器启动路径
- `_freeze_importlib.c` —— 用来把 importlib 的 bootstrap 代码冻结为生成的 C 数据

## 为什么这个目录重要

如果你想回答“操作系统最先从哪里进入 CPython”，就应该从这里开始。

最重要的一跳是：

- `python.c` → `Modules/main.c` 中的 `Py_BytesMain()`

在这之后，大部分真正有意思的控制流都会进入 `Modules/` 和 `Python/`。

## 这个目录如何连接到整棵源码树

- 进入 `Modules/main.c`，继续解释器启动流程
- 参与生成导入系统启动时使用的 frozen bootstrap 资源

## 推荐阅读顺序

1. `python.c`
2. `_freeze_importlib.c`
3. 然后切换到 `Modules/main.c`

## 这个目录回答哪些问题

- `main()` 定义在哪里？
- 进程是怎样交接给 CPython 运行时启动流程的？
- frozen importlib 数据是怎样生成的？
