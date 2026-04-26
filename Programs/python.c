/* Minimal main program -- everything is loaded from the library */

#include "Python.h"

#ifdef MS_WINDOWS
int
wmain(int argc, wchar_t **argv)
{
    // Windows 进程入口：把控制权交给解释器的宽字符启动入口。
    return Py_Main(argc, argv);
}
#else
int
main(int argc, char **argv)
{
    // Unix 风格进程入口：这是 `python helloworld.py` 最开始进入的 C 函数。
    // 这里本身不做解释器逻辑，只把 argv 转交给后续启动路径。
    return Py_BytesMain(argc, argv);
}
#endif
