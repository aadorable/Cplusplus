#include <iostream>
#include <map>

/////////////////////////////////////////////////////////////////
//该代码对 std::map 常用接口进行测试
//事件机制充分的体现出“框架”含义
//gtest 给我们提供了一组时机
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

TEST(TestMap, size){
    std::map<int, int> test_map;
    MakeMap(test_map);
    ASSERT_EQ(test_map.size(), 5);
}

TEST(TestMap, find){
    std::map<int, int> test_map;
    MakeMap(test_map);
    auto it = test_map.find(3);
    ASSERT_NE(it, test_map.end());
}

TEST(TestMap, operator_bracket){
    std::map<int, int> test_map;
    MakeMap(test_map);
    ASSERT_EQ(test_map[2], 1);
}
