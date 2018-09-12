#include <iostream>
#include <map>

/////////////////////////////////////////////////////////////////
//该代码对 std::map 常用接口进行测试
//事件机制充分的体现出“框架”含义
//gtest 给我们提供了一组时机
//去插入一些自定制的代码和逻辑
//1.每个 test 执行的开始和结束
//2.每个 test_case 执行的开始和结束
//3.全局的开始和结束
//为了让我们去构造数据和销毁数据
/////////////////////////////////////////////////////////////////
//
//框架指的是，需要给定一段逻辑，让框架决定什么时候调用这个逻辑
//函数指针
//仿函数
//多态
class TestMap : public testing::Test{
public:
    void SetUp(){
        //每个TEST初始化所调用的函数
        std::cout << "SetUp" << std::endl;
        test_map.insert(std::make_pair(1, 1));
        test_map.insert(std::make_pair(2, 1));
        test_map.insert(std::make_pair(3, 1));
        test_map.insert(std::make_pair(4, 1));
        test_map.insert(std::make_pair(5, 1));
    }

    void TearDown(){
        //每个TEST释放时所调用的函数
        std::cout << "TearDown" << std::endl;
        test_map.clear();
    }

    static void SetUpTestCase(){
        std::cout << "SetUpTestCase" << std::endl;
    }

    static void TearDownTestCase(){
        std::cout << "TearDownTestCase" << std::endl;
    }

    std::map<int, int> test_map;
};

TEST_F(TestMap, size){
    std::cout << "Test size beg" << std::endl;
    ASSERT_EQ(test_map.size(), 5);
    std::cout << "Test size end" << std::endl;
}

TEST_F(TestMap, find){
    std::cout << "Test find beg" << std::endl;
    auto it = test_map.find(3);
    ASSERT_NE(it, test_map.end());
    std::cout << "Test find beg" << std::endl;
}

TEST_F(TestMap, operator_bracket){
    std::cout << "Test operator_bracket beg" << std::endl;
    ASSERT_EQ(test_map[2], 1);
    std::cout << "Test operator_bracket beg" << std::endl;
}
