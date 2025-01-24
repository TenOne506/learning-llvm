### LLVM学习笔记

先从API开始学起，官网那个万花筒语言的教程已经过了一边，其中涉及的编译原理，大概也都了解了。

其中的`.clang-format`文件用来保持代码风格的，也是用来格式化代码的。

这个代码编译由CMake管理，并且使用`clang`来编译。之前并没怎么使用过CMake，但是把文件目录结构丢给大模型就可以了。

编译命令
```bash
mkdir build
cd build
cmake ..
make
```
并且尝试使用`xmake`来构建，在`xmake`分支下，包含了`xmake`的构建脚本。

只需在根目录下执行
```bash
xmake
```