# Lib/ 目录导读

这个目录包含标准库中用纯 Python 实现的部分。

## 这里放了什么

`Lib/` 是 CPython 的 Python 层。它包括：

- 纯 Python 标准库模块
- 包在 C 扩展模块外层的 Python 包装层
- 测试支持与回归测试（`Lib/test/`）
- importlib 的实现代码
- 随 Python 一起分发的工具和辅助包

## 重要职责

- 定义绝大多数用户可见的标准库行为
- 在底层 C 模块之上提供 Python 包装层
- 承载导入机制的 Python 实现部分
- 提供解释器和标准库的测试集

## 关键区域

- `importlib/` —— 用 Python 实现的导入系统
- `collections/`、`asyncio/`、`concurrent/`、`email/`、`json/` 等 —— 各类标准库包
- `test/` —— 运行时与标准库行为的回归测试套件
- `encodings/` —— codec 注册表和标准编码实现
- `site.py` —— `site` 初始化支持

## 为什么这个目录对解释器阅读者很重要

虽然 CPython 的核心是用 C 写的，但 Python 的大量可观察行为都活在这里。导入系统就是一个典型例子：底层 bootstrap 从 C 开始，但很多具体语义都在 `Lib/importlib/_bootstrap.py` 及相关模块中实现。

## 这个目录如何连接到整棵源码树

- `Modules/` 提供这里许多模块依赖的 C 扩展
- `Python/import.c` 会 bootstrap 到 `Lib/importlib/`
- `Doc/` 记录了这里许多模块对外暴露的行为

## 推荐阅读顺序

1. `importlib/_bootstrap.py`
2. `site.py`
3. `test/`，从行为导向的示例理解运行时预期

## 这个目录回答哪些问题

- 标准库里哪些模块是纯 Python 实现的？
- bootstrap 之后，导入机制具体在哪里实现？
- 解释器和标准库的回归测试放在哪里？
