## CPPhelper

> 一些关于 CPP 的零碎知识



### g++ 编译过程

1. 预处理阶段：`g++ -E` 让 g++ 在预处理之后停止编译，主要处理**头文件和宏** 
2. 编译阶段：`g++ -S` 只进行编译而不进行汇编，最终 `-o *.s` 生成**汇编代码**
3. 汇编阶段：`g++ -c` 生成目标代码 `-o *.o`，也就是**二进制代码**
4. 链接阶段：`g++ main.o test.o -o a.out` 利用目标代码生成**可执行文件**

> [g++编译详解__g++ 编译](https://blog.csdn.net/Three_dog/article/details/103688043)



**可变参数**

C++  可变参数：[C++函数可变参数-C++函数参数三个点](https://haicoder.net/cpp/cpp-function-varlist.html)



### CMakeLists && Makefile

<img src="https://img-blog.csdnimg.cn/b09e5d64b63f4f2594aed9910d776916.png" style="zoom:80%;"/>

  
#### 1. CMakeLists

CMake是一种跨平台编译工具，比make更为高级，使用起来要方便得多。CMake主要是编写CMakeLists.txt文件，然后用cmake命令将CMakeLists.txt文件转化为make所需要的makefile文件，最后用make命令编译源码生成可执行程序或共享库（so(shared object)）。因此CMake的编译基本就两个步骤：cmake 和 make



**实际例子**

假设主目录的结构如下：

```shell
.
├── build
├── CMakeLists.txt
├── include
│   └── b.h
└── src
    ├── b.c
    └── main.c
```



```cmake
# 1.cmake verson，指定cmake版本
cmake_minimum_required(VERSION 3.2)

# 2.project name，指定项目的名称，一般和项目的文件夹名称对应
PROJECT(test_sqrt)

# 3.head file path，头文件目录
INCLUDE_DIRECTORIES(include)

# 4.source directory，获取 src 文件夹下面所有的.cpp/.c/.cc文件，并赋值给变量 DIR_SRCS
AUX_SOURCE_DIRECTORY(src DIR_SRCS)

# 5.set environment variable，设置环境变量，编译用到的源文件全部都要放到这里，否则编译能够通过，但是执行的时候会出现各种问题，比如"symbol lookup error xxxxx , undefined symbol"
SET(TEST_MATH ${DIR_SRCS})

# 6.add executable file，添加要编译的可执行文件
ADD_EXECUTABLE(${PROJECT_NAME} ${TEST_MATH})

# 7.add link library，添加可执行文件所需要的库，比如我们用到了libm.so（命名规则：lib+name+.so），就添加该库的名称
TARGET_LINK_LIBRARIES(${PROJECT_NAME} m)
```

参考：

- https://www.cnblogs.com/cv-pr/p/6206921.html
- [【CMake】CMakeLists.txt的超傻瓜手把手教程（附实例源码）](https://blog.csdn.net/qq_38410730/article/details/102477162)



**相关命令**

```cmake
# 本CMakeLists.txt的project名称
# 会自动创建两个变量，PROJECT_SOURCE_DIR和PROJECT_NAME
# ${PROJECT_SOURCE_DIR}：本CMakeLists.txt所在的文件夹路径
# ${PROJECT_NAME}：本CMakeLists.txt的project名称
project(xxx)

# 获取路径下所有的.cpp/.c/.cc文件，并赋值给变量中
aux_source_directory(路径 变量)

# 给文件名/路径名或其他字符串起别名，用${变量}获取变量内容
set(变量 文件名/路径/...)

# 添加编译选项
add_definitions(编译选项)

# 打印消息
message(消息)

# 编译子文件夹的CMakeLists.txt
add_subdirectory(子文件夹名称)

# 将.cpp/.c/.cc文件生成.a静态库
# 注意，库文件名称通常为libxxx.so，在这里只要写xxx即可
add_library(库文件名称 STATIC 文件)

# 将.cpp/.c/.cc文件生成可执行文件
add_executable(可执行文件名称 文件)

# 规定.h头文件路径
include_directories(路径)

# 规定.so/.a库文件路径
link_directories(路径)

# 对add_library或add_executable生成的文件进行链接操作
# 注意，库文件名称通常为libxxx.so，在这里只要写xxx即可
target_link_libraries(库文件名称/可执行文件名称 链接的库文件名称)
```

参考：

- https://blog.csdn.net/afei__/article/details/81201039
- [CMake Reference Documentation — CMake 3.25.0-rc4 Documentation](https://cmake.org/cmake/help/v3.25/)



**gcc/g++ 链接 .so/.a 文件**

- .so 是 linux 的动态库，对应 windows 上的 .dll
- .a 是 linux 的静态库，对应 windows 上的 .lib

```shell
# 当前文件夹下面有库文件 libsource.a/libsource.so
g++ app.cc -L. -lsource -o app
```

参考：https://blog.csdn.net/FenDouZuoQingChun/article/details/103185861



#### 2. Makefile

make 命令执行时，需要一个 Makefile 文件，以告诉 make 命令需要怎么样的去编译和链接程序。

首先，我们用一个示例来说明 Makefile 的书写规则。以便给大家一个感兴认识。这个示例来源于 GNU 的 make 使用手册，在这个示例中，我们的工程有8个 C 文件，和3个头文件，我们要写一个Makefile 来告诉 make 命令如何编译和链接这几个文件。我们的规则是：

1. 如果这个工程没有编译过，那么我们的所有 C 文件都要编译并被链接。
2. 如果这个工程的某几个C文件被修改，那么我们只编译被修改的 C 文件，并链接目标程序。
3. 如果这个工程的头文件被改变了，那么我们需要编译引用了这几个头文件的 C 文件，并链接目标程序。

> 目前来说一般都是编写 CMakeLists，然后 cmake 生成 Makefile，所以先掌握 cmake 相关语法，makefile 相关语法先了解



**一个文件**

```makefile
target: prerequisites	# 目标文件（可以是目标文件: .o .obj 等，也可以是执行文件，还可以是 label）： 依赖项（生成 target 所需要的文件或是目标）
	command	# 命令（可以是任意的 shell 命令）
	
hello: hello.c
	gcc hello.c -o hello
```



**多个文件**

> bar.c包含求数组中最大值函数，foo.c包含求数组中最小值函数，各自对应的头文件是bar.h、foo.h，main_max调用求最大值函数来打印最大值，main_min调用求最小值函数打印最小值

```makefile
CC = gcc
CFLAGS = -O -Wall -m64 -std=gnu89
LIBS = -lm

all: main_max main_min   # 必须写成all这样的形式，否则只会生成前一个可执行文件main_max
 
main_max: main_max.c bar.o foo.o
    $(CC) main_max.c bar.o foo.o -o main_max
 
main_min: main_min.c bar.o foo.o
    $(CC) main_min.c bar.o foo.o -o main_min

foo.o: foo.c
    $(CC) -c foo.c

bar.o: bar.c
    $(CC) -c bar.c

.PHONY: clean # 为了避免与名为clean的文件冲突，最好在 clean 前面加 .PHONY
clean:
    rm *.o main_max main_min
```

在定义好依赖关系后，后续的那一行定义了如何生成目标文件的操作系统命令，一定要以一个Tab键作为开头。记住，make 并不管命令是怎么工作的，他只管执行所定义的命令。**make 会比较 targets 文件和 prerequisites 文件的修改日期，如果 prerequisites 文件的日期要比 targets 文件的日期要新，或者target不存在的话，make就会执行后续定义的命令。否则就会跳过。**



**参考：**

- [跟我一起写 Makefile（一）](https://blog.csdn.net/haoel/article/details/2886)

- [[收藏\]跟我一起写 Makefile (共 14 篇)](https://blog.csdn.net/coofucoo/article/details/446111)

- [:star2: Make 命令教程 - 阮一峰的网络日志 (ruanyifeng.com)](https://www.ruanyifeng.com/blog/2015/02/make.html)

- [Make Options -- GNU Make Manual](https://www.gnu.org/software/make/manual/html_node/Options-Summary.html#Options-Summary)
