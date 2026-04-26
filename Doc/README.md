# Doc/ 目录导读

这个目录包含 Python 官方文档的源码。

现有的 `Doc/README.rst` 仍然是构建文档的权威说明；这份 Markdown 文件则更像一份给源码阅读者用的简短导航。

## 这里放了什么

- Python 语言参考手册
- 标准库参考手册
- C API 参考文档
- 扩展 / 嵌入指南
- 教程、HOWTO、FAQ 和 What's New 文档

## 对解释器阅读者最重要的子目录

- `reference/` —— 语言语义，例如执行模型和导入系统
- `c-api/` —— 对象模型、type object、引用计数、初始化、GC、导入 API
- `extending/` —— 扩展和嵌入 Python
- `whatsnew/` —— 每个版本的变化摘要

## 对核心机制学习最相关的文件

- `reference/executionmodel.rst`
- `reference/import.rst`
- `c-api/init.rst`
- `c-api/refcounting.rst`
- `c-api/typeobj.rst`
- `c-api/gcsupport.rst`

## 这个目录如何连接到整棵源码树

- 记录了 `Python/`、`Objects/`、`Modules/` 和 `Lib/` 中实现的对外行为
- 解释了对应源码文件所实现的 API 和语义

## 推荐阅读顺序

1. `README.rst`，了解文档构建流程
2. `reference/executionmodel.rst`
3. `reference/import.rst`
4. `c-api/init.rst`
5. `c-api/typeobj.rst`
6. `c-api/refcounting.rst`
7. `c-api/gcsupport.rst`
