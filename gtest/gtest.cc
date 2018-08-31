////////////////////////////////////////////////////////
////以下代码为待测试代码
////////////////////////////////////////////////////////
//#include <iostream>
//#include <stdio.h>
//
//int Abs(int x){
//    return x > 0 ? x : -x;
//}
//
////////////////////////////////////////////////////////
////以下代码为单元测试代码
////////////////////////////////////////////////////////
//void TestAbs(){
//    int ret = Abs(10);
//    if(ret == 10){
//        printf("Test OK\n");
//    }
//    else{
//        printf("Test NO\n");
//    }
//    ret = Abs(-10);
//    if(ret == 10){
//        printf("Test OK\n");
//    }
//    else{
//        printf("Test NO\n");
//    } 
//    ret = Abs(0);
//    if(ret == 0){
//        printf("Test OK\n");
//    }
//    else{
//        printf("Test NO\n");
//    } 
//}
//
//int main(){
//    TestAbs();
//    return 0;
//}

int Abs(int x){
    return x > 0 ? x : -x;
}

#include <gtest/gtest.h>

// C语言中的 assert 断言失败，结果是什么样的？
// 致命断言：如果断言失败，当前的TEST宏中剩余的代码就不执行了
// 非致命断言：如果断言失败，TEST宏剩余的代码仍然继续执行

TEST(TestAbs, TestAbs){
    ASSERT_TRUE(Abs(10) == 10) << "Abs(10) = " << Abs(10);   //日志，失败时打印
    ASSERT_EQ(Abs(10), 10);              //不用打印日志也能发现错误
    ASSERT_TRUE(Abs(-10) == 10);
    ASSERT_TRUE(Abs(0) == 0);
}
