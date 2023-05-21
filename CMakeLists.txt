cmake_minimum_required(VERSION 3.20)
project(base_code)

set(CMAKE_CXX_STANDARD 14)

#add_executable(base_code main.cpp)

# 遍历项目根目录以及二级三级目录下所有的 .cpp 文件
# 参考：https://zhuanlan.zhihu.com/p/277990960
file (GLOB files *.cpp *.c
        */*.cpp */*.c
        */*/*.cpp */*/*.c)

foreach (file ${files})
#    string(REGEX REPLACE ".+/(.+)\\..*" "\\1" exe ${file})
    string(REGEX REPLACE ".+/(.+)/(.+)\\..*" "\\1_\\2" exe ${file})
    add_executable (${exe} ${file})
    message (\ \ \ \ --\ ${exe}.cpp\ will\ be\ compiled\ to\ ${exe})
endforeach ()
