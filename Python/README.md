# Python/ 目录导读

这个目录包含 CPython 的核心运行时实现。

旧的 `Python/README` 仍然保留；这份文件是在“源码阅读者”视角下对它的扩展。

## 这里放了什么

`Python/` 是解释器共享运行时行为的实现地。如果说 `Objects/` 定义了值“是什么”，那么 `Python/` 定义的就是解释器“如何运行”。

## 重要职责

- 解释器启动与关闭
- 线程状态与解释器状态管理
- 源码执行辅助逻辑
- 编译流水线
- 字节码求值
- 导入系统底层连接逻辑
- 异常、warnings、marshal 以及其他运行时服务

## 关键文件

### 启动与生命周期

- `pylifecycle.c` —— 运行时和解释器的初始化 / 终结
- `pystate.c` —— `PyInterpreterState`、`PyThreadState`、GIL 状态辅助逻辑
- `sysmodule.c` —— `sys` 模块初始化与支持代码

### 解析 / 编译 / 执行

- `pythonrun.c` —— 负责解析、编译并执行代码的高层辅助入口
- `compile.c` —— AST 到 `PyCodeObject`
- `symtable.c` —— 编译阶段使用的符号表构建
- `ast.c` / `Python-ast.c` —— AST 辅助逻辑与生成的 AST 支持代码

### 执行引擎

- `ceval.c` —— 字节码求值循环与 frame 执行
- `frame.c` —— frame 对象支持逻辑
- `traceback.c` —— traceback 机制

### 导入 / 序列化 / 运行时服务

- `import.c` —— 导入钩子、内建模块注册、底层导入支持
- `importdl.c` —— 扩展模块的动态加载支持
- `marshal.c` —— 字节码 / 代码对象序列化支持
- `_warnings.c` —— warnings 实现

## 这个目录如何连接到整棵源码树

- `Programs/` 和 `Modules/main.c` 会把启动流程推进到 `pylifecycle.c`
- `Parser/` 产出的解析结果会被 `pythonrun.c` 和 `compile.c` 消费
- `Objects/` 提供这里几乎到处都会用到的具体运行时类型
- `Include/` 为这些子系统提供公开和内部声明
- `Lib/importlib/` 实现了 `import.c` bootstrap 之后的大量导入语义

## 推荐阅读顺序

1. `pylifecycle.c`
2. `pystate.c`
3. `pythonrun.c`
4. `compile.c`
5. `ceval.c`
6. `import.c`

## 这个目录回答哪些问题

- `Py_InitializeFromConfig()` 是怎样工作的？
- `PyCodeObject` 从哪里来？
- 真正执行字节码的是哪个函数？
- 解释器状态和线程状态是怎样表示的？
- 底层导入状态是怎样初始化的？
