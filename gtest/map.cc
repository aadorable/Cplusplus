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

    std::map<int, int> test_map;
};

TEST_F(TestMap, size){
    ASSERT_EQ(test_map.size(), 5);
}

TEST_F(TestMap, find){
    auto it = test_map.find(3);
    ASSERT_NE(it, test_map.end());
}

TEST_F(TestMap, operator_bracket){
    ASSERT_EQ(test_map[2], 1);
}
