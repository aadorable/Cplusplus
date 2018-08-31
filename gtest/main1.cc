#include <iostream>
#include <gtest/gtest.h>

//////////////////////////////////////////////////
//以下代码是待测试代码
/////////////////////////////////////////////////
void Function(){
    int* p = NULL;
    *p = 100;
}

//json(非常重要的功能：序列化)
//优势：好看，可读性好
//劣势：效率比较低（存储了很多的冗余数据），占用的空间和带宽比较多
//protobuf:序列化库（google出品）
//优势：效率比较高
//劣势：可读性差
/////////////////////////////////////////////////
//以下代码是测试代码
////////////////////////////////////////////////
TEST(FuncDeathTest, Func){
    //第二个参数是正则表达式
    ASSERT_DEATH(Func(), "");
}