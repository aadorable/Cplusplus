#include <iostream>
#include <map>
#include "gtest/gtest.h"

////TestCase 中可以包含一组 Test
////参数不是字符串
//TEST(TestCaseHello, TestHello){
//    //TEST 宏内部可以写所有的合法的 C++ 代码
//    std::cout << "hello" << std::endl;
//}

int main(int argc, char* argv[]){
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
