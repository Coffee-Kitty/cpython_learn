# Parser/ 目录导读

这个目录包含 CPython 前端解析相关组件：分词器、解析器支持代码，以及与 AST 生成有关的输入定义。

## 这里放了什么

`Parser/` 负责把源代码文本转换成可供 AST 和编译阶段消费的解析结构。

这棵目录反映的是一个“过渡时期”的 CPython 布局：旧的解析器机制和 PEG 解析器组件同时存在。

## 重要职责

- 分词（`tokenizer.c`）
- 解析器支持逻辑（`parser.c`、`parsetok.c`）
- `pegen/` 下的 PEG 解析器实现
- 通过 `Python.asdl` 提供 AST 结构定义输入

## 关键文件

- `tokenizer.c` / `tokenizer.h` —— 词法分析支持
- `parsetok.c` —— parser / tokenizer 集成辅助逻辑
- `parser.c` / `parser.h` —— 解析器支持代码
- `Python.asdl` —— AST 节点生成所用的结构描述
- `pegen/pegen.c` —— PEG 解析器支持实现
- `pegen/peg_api.c` —— PEG 解析器 API 入口支持

## 这个目录在执行流水线中的位置

高层流程是：

1. 源代码文本在这里被分词
2. 解析器机制产出 parser 输出 / AST 输入
3. `Python/pythonrun.c` 和 `Python/compile.c` 再消费这些结果

## 历史说明

在这个仓库快照里，`pythonrun.c` 仍然可以在 PEG 解析路径和旧解析路径之间切换。因此，如果你想理解 CPython 在解析器迁移期是怎样演进的，这个目录会特别有价值。

## 这个目录如何连接到整棵源码树

- `Grammar/` 提供解析器生成工具使用的语法定义
- `Python/compile.c` 会消费解析之后生成的 AST 结构
- `Tools/peg_generator/` 包含相关的解析器生成工具

## 推荐阅读顺序

1. `tokenizer.c`
2. `parsetok.c`
3. `pegen/pegen.c`
4. `Python.asdl`

## 这个目录回答哪些问题

- CPython 是怎样把源代码拆成 token 的？
- PEG 解析器具体放在哪里？
- AST 节点的形状是怎样描述的？
- 哪些 parser 层文件还因为历史兼容而被保留？
