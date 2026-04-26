# CPython 中文学习导读

CPython 是 Python 的参考实现，主要使用 C 编写，并带有一个庞大的标准库（其中很大一部分用 Python 编写）。

这份 `README.zh-CN.md` 整合了当前仓库里的三类中文内容：

- 源码树导读
- 7 天学习计划与执行链路讲解
- 本地构建与语法实验记录

如果你的目标不是“只把它编译出来”，而是“真正学会这个项目”，这份文档就是入口。

真正的上游构建与贡献入口仍然是 `README.rst`。

## 如何使用本文

可以按下面三种方式阅读：

1. 想快速知道仓库怎么分层：先读“Architecture”和“目录导读”
2. 想知道代码到底怎么跑起来：直接读“一段 Python 代码到底怎么执行”
3. 想系统入门：按“7 天学习计划”顺序推进

建议同时搭配下面几个文件：

- `README.rst`：官方构建、测试、贡献说明
- `Programs/README.md`
- `Modules/README.md`
- `Python/README.md`
- `Objects/README.md`
- `Parser/README.md`

当前这个仓库对应的是 `Python 3.9.0` 时代的源码树，因此你会看到 3.9 阶段的启动流程、PEG 解析器和执行器实现。

## Install

官方构建说明请以 `README.rst` 为准。

典型的类 Unix 平台构建流程如下：

```bash
./configure
make
make test
```

### 本地 macOS 调试构建笔记

如果你只是想本地读源码、看解释器行为、跑一些不依赖完整系统库的实验，这套本地构建方式可以作为参考：

```bash
arch -x86_64 /bin/zsh

CPPFLAGS="-I$(brew --prefix zlib)/include" \
LDFLAGS="-L$(brew --prefix zlib)/lib" \
./configure --with-openssl=$(brew --prefix openssl) --with-pydebug

make -j2 -s
```

可以快速验证：

```bash
./python.exe -V
./python.exe -c "print('ok')"
```

如果你做的是这些事情，这种构建通常已经够用了：

- 阅读源码
- 跑不依赖 `ssl` 的 benchmark
- 看 `ceval` / object model / parser / gc / dict / list 等核心机制

## Quickstart

如果你的目标是理解实现原理，而不是先把它完整构建成可分发解释器，建议从这里开始：

1. `Programs/python.c` —— 进程入口
2. `Modules/main.c` —— CLI / 启动流程
3. `Python/pylifecycle.c` —— 解释器初始化与销毁
4. `Python/pythonrun.c` —— 解析 / 编译 / 执行主路径
5. `Python/compile.c` —— AST 到代码对象
6. `Python/ceval.c` —— 字节码求值循环
7. `Include/object.h` 与 `Include/cpython/object.h` —— 对象 / 类型布局
8. `Objects/typeobject.c` —— 类型系统核心机制
9. `Modules/gcmodule.c` —— 循环垃圾回收器
10. `Python/import.c` 与 `Lib/importlib/_bootstrap.py` —— 导入系统启动与引导

## Usage

把这份文档当作代码库地图来使用：

- 构建、测试与贡献流程请先阅读 `README.rst`
- 大目录下新增的 `README.md` 可帮助你进一步理解各个子系统
- 如果你想最快进入解释器核心，这份文档里的文件路径就是最短路线

## Configuration

运行时配置主要流经以下位置：

- `Modules/main.c` —— 命令行解析与启动流程
- `Python/pylifecycle.c` —— `Py_InitializeFromConfig()` 及相关生命周期 API
- `Modules/config.c` —— 内建模块表（`_PyImport_Inittab`）

构建期配置主要流经以下位置：

- `configure` / `configure.ac`
- `Makefile.pre.in`
- `PCbuild/`、`Mac/` 等平台相关目录

## Architecture

### 全局图景

可以把 CPython 看成 6 个互相协作的子系统：

1. 启动与生命周期 —— 把一个操作系统进程变成一个已配置好的解释器
2. 解析与编译 —— 把源代码文本变成 AST，再变成字节码 / 代码对象
3. 执行引擎 —— 执行栈帧与字节码指令
4. 对象模型 —— 所有运行时值都以 `PyObject *` 形式表示
5. 内存管理 —— 引用计数加循环垃圾回收
6. 导入系统与扩展边界 —— 加载 Python 模块、内建模块与 C 扩展

### 核心机制地图

#### 1. 启动路径

- `Programs/python.c` 调用 `Py_BytesMain()`
- `Modules/main.c` 处理命令行启动并调用 `Py_InitializeFromConfig()`
- `Python/pylifecycle.c` 初始化运行时、解释器状态、线程、builtins 与导入状态

#### 2. 解析 / 编译 / 执行路径

- `Python/pythonrun.c` 负责把源码送入解析 API
- `Parser/` 包含分词器与解析器实现
- `Python/compile.c` 负责把 AST 编译为 `PyCodeObject`
- `Python/ceval.c` 负责通过帧求值执行代码对象

#### 3. 对象模型

- `Include/object.h` 定义基础对象布局与引用计数辅助宏
- `Include/cpython/object.h` 定义 `PyTypeObject` 及相关方法表
- `Objects/` 实现内建类型，例如 dict、list、tuple、unicode、int 与异常对象

#### 4. 内存管理

- 引用计数通过 `Py_INCREF`、`Py_DECREF` 等宏实现
- `Modules/gcmodule.c` 为容器对象实现循环垃圾回收

#### 5. 导入系统

- `Python/import.c` 初始化低层导入状态与内建模块装配逻辑
- `Lib/importlib/_bootstrap.py` 用 Python 实现了大部分导入语义
- `Python/importlib.h` 保存启动早期使用的 frozen bootstrap 副本

### 目录导读

| 目录 | 作用 |
| --- | --- |
| `Programs/` | 可执行入口程序与辅助程序 |
| `Modules/` | 解释器主程序、内建模块、C 扩展模块 |
| `Python/` | 运行时核心：生命周期、编译器、求值循环、导入底层 |
| `Objects/` | 内建对象实现 |
| `Include/` | 公共与内部 C 头文件 |
| `Parser/` | 分词器、解析器、AST 前端相关代码 |
| `Lib/` | 纯 Python 标准库 |
| `Doc/` | Python 官方文档源码 |
| `Tools/` | 维护与生成工具 |
| `Grammar/` | 解析器工具链使用的语法定义 |

如需进一步了解，请继续阅读这些目录下的本地 `README.md`。

## 7 天学习计划

这份计划不是“7 天学完 CPython”，而是“7 天建立继续深挖的正确地图”。

### Day 1：先把地图看明白

目标：知道顶层目录各管什么，以及启动主线大概经过哪里。

优先阅读：

- `README.zh-CN.md`
- `Programs/README.md`
- `Modules/README.md`
- `Python/README.md`
- `Objects/README.md`
- `Parser/README.md`

再读这些关键入口：

- `Programs/python.c`
- `Modules/main.c`

今天要看懂：

- 为什么 `Programs/python.c` 只有很少几行，却非常关键
- 为什么启动逻辑主要不在 `Programs/`，而在 `Modules/main.c`
- 为什么 `Modules/` 不只是“扩展模块目录”

今天的输出：

- 用你自己的话写出顶层目录职责图
- 写出启动主线的前 5 跳

如果你想在 Day 1 就先摸一遍真实代码，建议加一个最小例子：

```python
# helloworld.py
print("hello world")
```

然后假设你执行：

```bash
./python.exe helloworld.py
```

你第一遍应该先抓“主调用链”，不要试图一次看懂全部细节。可以先沿着下面这条线摸：

1. `Programs/python.c:main()`
2. `Programs/python.c:Py_BytesMain()`
3. `Modules/main.c:pymain_main()`
4. `Modules/main.c:pymain_init()`
5. `Python/pylifecycle.c:Py_InitializeFromConfig()`
6. `Modules/main.c:Py_RunMain()`
7. `Modules/main.c:pymain_run_python()`
8. `Modules/main.c:pymain_run_file()`
9. `Python/pythonrun.c:PyRun_FileExFlags()`
10. `Parser/pegen/peg_api.c:PyPegen_ASTFromFileObject()`
11. `Parser/pegen/pegen.c:_PyPegen_run_parser_from_file_pointer()`
12. `Python/pythonrun.c:run_mod()`
13. `Python/compile.c:PyAST_CompileObject()`
14. `Python/pythonrun.c:run_eval_code_obj()`
15. `Python/ceval.c:PyEval_EvalCode()`
16. `Python/ceval.c:_PyEval_EvalCode()`
17. `Objects/frameobject.c:_PyFrame_New_NoTrack()`
18. `Python/ceval.c:_PyEval_EvalFrameDefault()`

可以先把这条链粗略分成 5 段来记：

- 进程入口：`Programs/python.c`
- 启动与分发：`Modules/main.c`
- 初始化解释器：`Python/pylifecycle.c`
- 解析和编译：`Python/pythonrun.c` + `Parser/` + `Python/compile.c`
- 真正执行：`Python/ceval.c`

你在 Day 1 不需要读懂每个函数体，只要先回答下面这几个问题就够了：

- 操作系统先把控制权交给谁？
- 哪个函数决定这是“运行脚本文件”而不是 `-c` 或 `-m`？
- 哪个函数把源码文件送进 parser？
- 哪个函数把 AST 编译成 `PyCodeObject`？
- 哪个函数最终开始解释执行字节码？

如果你能把 `helloworld.py` 的这条链从头顺到 `_PyEval_EvalFrameDefault()`，那 Day 1 的“地图感”就已经建立起来了。

### Day 2：学解释器是怎么“活过来”的

目标：理解 runtime、interpreter、thread state 是怎么初始化的。

优先阅读：

- `Python/pylifecycle.c`
- `Python/pystate.c`

重点函数：

- `Py_InitializeFromConfig`
- `pyinit_core`
- `pyinit_main`
- `init_interp_main`

今天要看懂：

- 哪一步只是“核心初始化”
- 哪一步才让解释器进入“可真正运行 Python 代码”的状态
- `sys`、importlib、标准流、信号、`__main__` 分别在什么阶段准备好

今天的输出：

- 画出“core initialized” 和 “fully initialized” 的区别

### Day 3：学源码如何变成 AST

目标：理解 tokenizer、parser、AST 之间的边界。

优先阅读：

- `Grammar/python.gram`
- `Parser/tokenizer.c`
- `Parser/pegen/peg_api.c`
- `Parser/pegen/pegen.c`

重点函数：

- `PyTokenizer_FromFile`
- `PyPegen_ASTFromFileObject`
- `_PyPegen_run_parser_from_file_pointer`

今天要看懂：

- 文本输入是怎样进入 tokenizer 的
- PEG parser 怎么从 token 流构造 AST
- 为什么 parser 相关代码和 compiler 相关代码是两层不同抽象

今天的输出：

- 写出“文本 -> token -> AST” 的三层关系

### Day 4：学 AST 如何变成 `PyCodeObject`

目标：理解 symbol table、basic block、assemble 三步。

优先阅读：

- `Python/compile.c`
- `Python/symtable.c`

重点函数：

- `PyAST_CompileObject`
- `PySymtable_BuildObject`
- `compiler_mod`
- `assemble`

今天要看懂：

- 为什么编译前要先做符号表分析
- `global` / `nonlocal` / 闭包 / 局部变量信息是在哪一步确定的
- 为什么 `compiler_mod()` 不是直接输出字节码，而是先组织 basic blocks
- 为什么 `assemble()` 是最终把结构化中间结果压成 code object 的地方

今天的输出：

- 用 5 句话解释 `PyAST_CompileObject()` 的流程

### Day 5：学字节码是怎么执行的

目标：理解 frame、locals、value stack、eval loop。

优先阅读：

- `Python/pythonrun.c`
- `Objects/frameobject.c`
- `Include/internal/pycore_ceval.h`
- `Python/ceval.c`

重点函数：

- `run_mod`
- `run_eval_code_obj`
- `PyEval_EvalCode`
- `_PyEval_EvalCode`
- `_PyFrame_New_NoTrack`
- `_PyEval_EvalFrameDefault`

今天要看懂：

- 为什么执行前必须先创建 `PyFrameObject`
- locals、globals、builtins 分别挂在哪
- 为什么 `_PyEval_EvalFrameDefault()` 才是最核心的“解释执行循环”

今天的输出：

- 解释 `PyCodeObject` 和 `PyFrameObject` 的区别

### Day 6：学对象系统和内存管理

目标：理解为什么一切值都围绕 `PyObject *` 运转。

优先阅读：

- `Include/object.h`
- `Include/cpython/object.h`
- `Objects/object.c`
- `Objects/typeobject.c`
- `Objects/obmalloc.c`
- `Modules/gcmodule.c`

今天要看懂：

- 引用计数在对象头里如何体现
- `PyTypeObject` 如何驱动方法分发
- 引用计数和循环垃圾回收各自解决什么问题

今天的输出：

- 解释“引用计数解决不了循环引用，所以还需要 GC”

### Day 7：学导入系统和测试体系

目标：把启动、执行、标准库和测试串成闭环。

优先阅读：

- `Python/import.c`
- `Lib/importlib/_bootstrap.py`
- `Modules/config.c`
- `Lib/test/`
- `Lib/test/regrtest.py`

今天要看懂：

- 启动阶段 importlib 为什么有 bootstrap 特殊性
- 内建模块表是怎么接入解释器的
- 为什么测试目录是学习行为边界和历史 bug 的最好入口之一

今天的输出：

- 写出你眼中的 CPython 启动到执行全链路

## 一段 Python 代码到底怎么执行

下面按真正的控制流来讲，而不是按目录讲。

假设你运行的是：

```python
x = 1 + 2
print(x)
```

并通过下面这个命令启动：

```bash
./python.exe demo.py
```

### 1. 进程入口

入口在 `Programs/python.c`。

- 在类 Unix 平台，`main()` 调用 `Py_BytesMain()`
- 在 Windows 平台，`wmain()` 调用 `Py_Main()`

这个文件非常小，因为它只负责做一件事：把操作系统层面的参数交给 CPython 的真正启动逻辑。

### 2. 进入 `Modules/main.c`

`Py_BytesMain()` 和 `Py_Main()` 都会进入 `pymain_main()`。

`pymain_main()` 的逻辑非常朴素：

1. 调 `pymain_init()` 完成初始化
2. 如果初始化失败，直接退出
3. 如果成功，进入 `Py_RunMain()`

你可以把 `pymain_main()` 看成整个 CLI 启动流程的总调度。

### 3. `pymain_init()`：把命令行世界转换成解释器配置

`pymain_init()` 在 `Modules/main.c` 中完成下面几步：

1. `_PyRuntime_Initialize()`
2. 构造 `PyPreConfig`
3. `_Py_PreInitializeFromPyArgv()`
4. 构造 `PyConfig`
5. `PyConfig_SetArgv()` 或 `PyConfig_SetBytesArgv()`
6. `Py_InitializeFromConfig()`

它做的本质工作是：

- 读取命令行参数
- 准备解释器配置
- 把“还没初始化的 C 进程”推进到“准备创建 Python 运行时”的状态

### 4. `Py_InitializeFromConfig()`：真正初始化解释器

`Py_InitializeFromConfig()` 在 `Python/pylifecycle.c` 中。

它先做 `pyinit_core()`，再决定是否做 `pyinit_main()`。

#### `pyinit_core()` 负责什么

这是“核心初始化”阶段，主要做：

- 预初始化配置
- 拷贝和读取完整配置
- 创建第一个 interpreter state 和 thread state
- 建立 runtime 的核心结构

这时解释器已经“活了”，但还没有完全进入我们平时使用 Python 时的那种状态。

#### `pyinit_main()` / `init_interp_main()` 负责什么

这是“补全主解释器能力”的阶段，主要做：

- 完成 `sys` 初始化
- 初始化 importlib 外层能力
- 初始化编码系统
- 安装信号处理
- 初始化标准流
- 设置内建 `open`
- 挂上 `__main__`

只有完成这一步，解释器才算真正准备好去执行用户代码。

### 5. `Py_RunMain()`：开始执行用户请求的代码

初始化完成后，会回到 `Modules/main.c` 里的 `Py_RunMain()`。

它会调用 `pymain_run_python()`，并在执行结束后：

- 调 `Py_FinalizeEx()` 做收尾
- 处理未捕获的 `KeyboardInterrupt`
- 返回进程退出码

### 6. `pymain_run_python()`：决定到底跑什么

`pymain_run_python()` 是执行阶段的分发中心。

它会根据配置决定：

- `-c`：走 `pymain_run_command()`
- `-m`：走 `pymain_run_module()`
- 脚本文件：走 `pymain_run_file()`
- 没给脚本：走 `pymain_run_stdin()` / REPL

它还会处理 `sys.path[0]`，也就是决定导入时“当前入口路径”放在哪里。

### 7. `pymain_run_file()`：把脚本文件送进运行时

当你执行 `demo.py` 时，会走 `pymain_run_file()`。

这里会做几件很关键的事：

- 打开文件
- 检查是不是目录
- 处理首行跳过（如脚本 shebang）
- 调 `PyRun_AnyFileExFlags()`

这一步之后，控制权就逐步从“CLI 层”进入“解析 / 编译 / 执行层”。

### 8. `PyRun_FileExFlags()`：源码变 AST

`PyRun_FileExFlags()` 在 `Python/pythonrun.c` 中，是最重要的桥梁之一。

它的主流程是：

1. 把文件名变成 Python 字符串对象
2. 创建 `PyArena`
3. 调 parser，把源码变成 AST（`mod_ty`）
4. 调 `run_mod()`

注意这里有一个重要边界：

- parser 输出的是 AST
- compiler 才会把 AST 变成 `PyCodeObject`

### 9. parser 阶段：文本 -> token -> AST

如果使用 PEG parser，那么会走：

- `PyPegen_ASTFromFileObject()`
- `_PyPegen_run_parser_from_file_pointer()`

这条路径内部会先调用：

- `PyTokenizer_FromFile()`

所以你可以把这一阶段理解为：

1. 文件流进入 tokenizer
2. tokenizer 把字符流分解成 token 流
3. parser 根据 `Grammar/python.gram` 把 token 组织成 AST

到这里，`demo.py` 已经不再只是文本了，而是一棵抽象语法树。

### 10. compiler 阶段：AST -> `PyCodeObject`

拿到 AST 后，`run_mod()` 会调用 `PyAST_CompileObject()`。

`PyAST_CompileObject()` 可以概括成下面几步：

1. 处理 `__future__` 特性
2. 做 AST 级优化
3. 调 `PySymtable_BuildObject()` 构造符号表
4. 调 `compiler_mod()` 生成 basic blocks
5. 调 `assemble()` 把 basic blocks 组装成最终 code object

其中：

- `PySymtable_BuildObject()` 负责作用域分析
- `compiler_mod()` 负责根据 AST 生成结构化的中间结果
- `assemble()` 负责真正发射字节码和跳转偏移，并制作 `PyCodeObject`

也就是说，编译器不是“边遍历 AST 边吐最终字节码”这么简单，而是先做分析，再做结构化生成，最后统一装配。

### 11. `run_eval_code_obj()`：准备执行 code object

一旦 `PyCodeObject` 编译出来，`run_mod()` 会把它交给 `run_eval_code_obj()`。

`run_eval_code_obj()` 会做两件关键事情：

1. 如果 `globals` 中还没有 `__builtins__`，先补上
2. 调 `PyEval_EvalCode()` 进入执行器

这一步很重要，因为 Python 代码默认依赖 builtins，比如 `print()`、`len()`。

### 12. `PyEval_EvalCode()` / `_PyEval_EvalCode()`：创建 frame

`PyEval_EvalCode()` 是常见的外层入口，内部会进入 `_PyEval_EvalCode()`。

`_PyEval_EvalCode()` 做的关键工作包括：

- 调 `_PyFrame_New_NoTrack()` 创建 `PyFrameObject`
- 把参数填入 `f_localsplus`
- 处理 `*args` / `**kwargs`
- 最终调用 `_PyEval_EvalFrame()`

这时候，代码对象已经拥有了“执行现场”。

### 13. `_PyFrame_New_NoTrack()`：建立一次执行现场

frame 可以理解成“一次执行调用的上下文”。

`_PyFrame_New_NoTrack()` 会挂上这些关键数据：

- `f_code`：要执行的 code object
- `f_globals`：模块级全局命名空间
- `f_locals`：局部命名空间
- `f_builtins`：内建对象字典
- `f_back`：上一层 frame

所以：

- `PyCodeObject` 是“程序”
- `PyFrameObject` 是“程序这一次被执行时的现场”

### 14. `_PyEval_EvalFrame()`：跳到真正的求值函数

`_PyEval_EvalFrame()` 不是重逻辑实现，而是通过 `tstate->interp->eval_frame` 把控制权交给当前解释器配置的 frame evaluator。

默认情况下，这个指针在 `Python/pystate.c` 中被设为：

- `_PyEval_EvalFrameDefault`

这意味着通常最终都会进入默认字节码解释器。

### 15. `_PyEval_EvalFrameDefault()`：真正执行字节码

这就是 CPython 最经典的核心函数之一。

它负责：

- 读取当前 opcode 和 oparg
- 维护 value stack
- 处理局部变量、常量、名字查找
- 做跳转、调用、异常处理、返回值处理
- 在需要时处理信号、pending calls、GIL 让出等

你可以把它理解成一个巨大的“按指令解释执行”的循环。

简单说：

- compiler 决定“生成什么字节码”
- eval loop 决定“每条字节码怎么执行”

## 用一个最小例子串起来

假设源码是：

```python
x = 1 + 2
print(x)
```

你可以这样理解它的执行过程：

1. `Programs/python.c` 把进程入口交给 `Modules/main.c`
2. `Modules/main.c` 初始化解释器并决定要跑文件
3. `Python/pythonrun.c` 把文件送进 parser
4. `Parser/` 生成 AST
5. `Python/compile.c` 把 AST 编译成 `PyCodeObject`
6. `Python/ceval.c` 创建 frame 并逐条执行字节码
7. `print(x)` 通过 builtins 找到 `print`
8. 输出 `3`

如果你以后看到 `dis` 模块输出的字节码，脑中就应该能自动联想到：

- 这些指令来自 `compile.c`
- 这些指令由 `ceval.c` 执行

## Suggested reading paths

### A. “解释器是如何启动的？”

建议阅读：

- `Programs/python.c`
- `Modules/main.c`
- `Python/pylifecycle.c`
- `Python/pystate.c`

### B. “Python 代码是如何执行的？”

建议阅读：

- `Python/pythonrun.c`
- `Parser/pegen/pegen.c`
- `Python/compile.c`
- `Python/ceval.c`

### C. “为什么一切都是对象？”

建议阅读：

- `Include/object.h`
- `Include/cpython/object.h`
- `Objects/object.c`
- `Objects/typeobject.c`

### D. “模块是如何加载的？”

建议阅读：

- `Python/import.c`
- `Modules/config.c`
- `Lib/importlib/_bootstrap.py`
- `Python/importlib.h`

### E. “内存管理是如何工作的？”

建议阅读：

- `Include/object.h`
- `Modules/gcmodule.c`
- `Doc/c-api/refcounting.rst`
- `Doc/c-api/gcsupport.rst`

## 读源码时，永远盯住这 5 个问题

每读一个核心函数，都问自己：

1. 输入是什么？
2. 输出是什么？
3. 改了哪些运行时状态？
4. 错误路径怎么返回？
5. 下一跳交给谁？

只要这 5 个问题始终在，你就不容易迷失在巨量 C 代码里。

## 第一阶段必须吃透的 10 个文件

如果你只能优先读 10 个文件，就读这些：

1. `Programs/python.c`
2. `Modules/main.c`
3. `Python/pylifecycle.c`
4. `Python/pythonrun.c`
5. `Parser/pegen/peg_api.c`
6. `Parser/pegen/pegen.c`
7. `Python/compile.c`
8. `Python/symtable.c`
9. `Objects/frameobject.c`
10. `Python/ceval.c`

如果你把这 10 个文件真正串起来，你对 CPython 的理解就已经超过“只会跑源码和改几个关键字”的阶段了。

## 初学者最容易踩的坑

- 不要一上来就只看 `Python/ceval.c`
- 不要把 `Modules/` 误解成“只是模块实现”
- 不要忽略头文件，尤其是 `Include/object.h` 和内部头
- 不要把 parser、compiler、eval loop 混成一坨
- 不要只看 happy path，要一起看错误返回和资源释放

## 学完这 7 天，你应该能自测什么

如果你能流畅回答下面这些问题，说明你已经真正入门：

- `Py_BytesMain()` 和 `Py_RunMain()` 分别处在哪一层？
- `Py_InitializeFromConfig()` 为什么比 `Py_Initialize()` 更适合理解现代启动流程？
- parser 的输出为什么不是字节码，而是 AST？
- 为什么编译器要先做符号表分析？
- 为什么执行 `PyCodeObject` 前必须先创建 frame？
- `interp->eval_frame` 为什么是一个函数指针？

## 本地实验记录：修改关键字示例

下面这段内容整理自本地实验笔记，适合作为“怎么从源码修改语法并验证”的最小练习。

### 语法文件位置

```text
Grammar/
├── Tokens      ← 定义词法单元长什么样
├── python.gram ← 定义这些词怎样组成语法
└── Grammar     ← 历史遗留 / 兼容文件
```

这次实验改的是：

- `Grammar/python.gram`

目标是把 `coffeecat` 当成一个类似 `pass` 的语法关键字来实验。

### 基本步骤

修改语法文件后，重新生成 parser：

```bash
make regen-pegen
```

然后重新编译解释器。

### 验证思路

一个简单的验证方式是看 tokenize / parse 结果是否符合预期。

实验截图：

![添加关键字后的效果](assets/README-zh/image-2.png)

示例输出：

```bash
python -m tokenize -e test_tokens.py
0,0-0,0:            ENCODING       'utf-8'
1,0-1,3:            NAME           'def'
1,4-1,11:           NAME           'example'
1,11-1,12:          LPAR           '('
1,12-1,13:          RPAR           ')'
1,13-1,14:          COLON          ':'
1,14-1,15:          NEWLINE        '\n'
2,0-2,4:            INDENT         '    '
2,4-2,13:           NAME           'coffeecat'
2,13-2,14:          NEWLINE        '\n'
3,0-3,0:            DEDENT         ''
3,0-3,7:            NAME           'example'
3,7-3,8:            LPAR           '('
3,8-3,9:            RPAR           ')'
3,9-3,10:           NEWLINE        '\n'
4,0-4,0:            ENDMARKER      ''
```

这个实验的学习价值不在于“加了一个关键字”，而在于你会顺手摸到：

- `Grammar/` 在哪里起作用
- 修改语法后为什么要 `regen-pegen`
- parser 和 tokenizer 的边界在哪里

## Development

- 测试：`make test`
- 扩展测试：`make testall`
- 文档构建：`make -C Doc html`
- 代码风格 / 格式化：遵循 `README.rst` 与 Python Developer Guide 中的贡献流程

## 第二阶段怎么继续学

当你完成第一轮学习后，建议往这几个方向继续深入：

1. 对象系统：`Objects/typeobject.c`、`dictobject.c`、`listobject.c`
2. 内存管理：`Objects/obmalloc.c`、`Modules/gcmodule.c`
3. 导入系统：`Python/import.c`、`Lib/importlib/_bootstrap.py`
4. 测试体系：`Lib/test/`
5. C API：`Doc/c-api/`

如果你要长期学这个项目，最好给自己建立一个习惯：

- 每读完一个文件，就写下“它接什么、产什么、把控制权交给谁”

这会比无差别做笔记有效得多。

## 今日总结（2026-04-26）

今天的目标不是“把整个 CPython 看懂”，而是先把 `helloworld.py` 这条最小主执行链真正走通。这个目标已经完成。

### 今天确认的主调用链

这条链已经实际读过、加过注释，并且结合 debug 走过一遍：

1. `Programs/python.c:main()`
2. `Programs/python.c:Py_BytesMain()`
3. `Modules/main.c:pymain_main()`
4. `Modules/main.c:pymain_init()`
5. `Python/pylifecycle.c:Py_InitializeFromConfig()`
6. `Modules/main.c:Py_RunMain()`
7. `Modules/main.c:pymain_run_python()`
8. `Modules/main.c:pymain_run_file()`
9. `Python/pythonrun.c:PyRun_FileExFlags()`
10. `Parser/pegen/peg_api.c:PyPegen_ASTFromFileObject()`
11. `Parser/pegen/pegen.c:_PyPegen_run_parser_from_file_pointer()`
12. `Python/pythonrun.c:run_mod()`
13. `Python/compile.c:PyAST_CompileObject()`
14. `Python/pythonrun.c:run_eval_code_obj()`
15. `Python/ceval.c:PyEval_EvalCode()`
16. `Python/ceval.c:_PyEval_EvalCode()`
17. `Objects/frameobject.c:_PyFrame_New_NoTrack()`
18. `Python/ceval.c:_PyEval_EvalFrameDefault()`

### 今天最重要的理解

- `Programs/python.c` 只是进程入口，真正的 CLI 启动控制流在 `Modules/main.c`
- `Python/pythonrun.c` 是“运行桥接层”：它把文件、字符串、交互输入统一送进 parser / compile / eval
- `Parser/pegen/` 负责把源码文本变成 AST
- `Python/compile.c` 负责把 AST 变成 `PyCodeObject`
- `_PyEval_EvalCode()` 非常关键，因为它不是解释 opcode 的地方，但它负责把 `PyCodeObject` 变成“可执行现场”：创建 frame、填参数、处理默认值、闭包、`*args` / `**kwargs`
- `_PyEval_EvalFrameDefault()` 才是真正解释 opcode 的核心循环；如果要理解“字节码怎么跑”，这里是必须反复阅读和 debug 的地方

### 今天加过中文注释的关键文件

- `Programs/python.c`
- `Modules/main.c`
- `Python/pylifecycle.c`
- `Python/pythonrun.c`
- `Parser/pegen/peg_api.c`
- `Parser/pegen/pegen.c`
- `Python/compile.c`
- `Objects/frameobject.c`
- `Python/ceval.c`

这些注释的目标不是翻译每一行，而是让你在顺着主执行链 debug 时，能随时知道“这一步在整条链里的职责”。

### 今天调试环境上踩到并解决的问题

- VSCode 最开始停在 `_dyld_start`，那不是 CPython 错误，而是 debugger 停在了 macOS 动态加载器入口；后来已经改成更适合源码学习的启动方式
- 一开始运行 `helloworld.py` 时出现：

```text
Could not find platform dependent libraries <exec_prefix>
Consider setting $PYTHONHOME to <prefix>[:<exec_prefix>]
```

根因不是主链错误，而是只构建了 `python.exe`，没有把 `sharedmods` 一起构建出来，导致 `lib-dynload` 路径不完整

- 重新构建方式：

```bash
arch -x86_64 make -j2 -s python.exe sharedmods
```

之后 `helloworld.py` 已经可以直接运行并输出：

```text
hello world
```

这说明今天最核心的目标——把最小脚本从入口一路跑到执行器——已经完成。

### 下次继续时，最值得从哪里开始

如果下次继续，不要重新从 `Programs/python.c` 全部开始。最值得继续盯住的是这三个点：

1. `Python/pythonrun.c:PyRun_FileExFlags()` —— 文件输入真正进入 parser / compile / eval 的桥
2. `Python/ceval.c:_PyEval_EvalCode()` —— `PyCodeObject` 变成 frame 的入口
3. `Python/ceval.c:_PyEval_EvalFrameDefault()` —— 解释 opcode 的核心循环

如果把这三段再结合 debug 走深一轮，你对 CPython “代码究竟怎么跑起来”的理解就会非常扎实。
