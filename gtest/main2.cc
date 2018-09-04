#include <iostream>
#include <vector>
/////////////////////////////////////////////////////////////
//明确需求：实现一个简单的数据结构栈
//要求对这个栈进行对应操作的时候都能反馈成功失败
//
//TDD 的基本步骤：
//1.先写测试代码
//2.尝试编译运行测试代码
//3.如果测试不通过，就修改业务代码让测试通过
//4.如果测试通过，就继续编写新的测试代码
////////////////////////////////////////////////////////////

class Stack{
public:
    Stack() {}

    bool Push(int x){
        data_.push_back(x);
        return true;
    }

    bool Top(int* x) const{
        //如果当前栈为空栈，Top失败
        //如果当前栈不为空，Top成功
        if(data_.empty()){
            return false;
        }
        *x = data_.back();
        return true;
    }

    bool Pop(){
        if(data_.empty()){
            return false;
        }
        data_.pop_back();
        return true;
    }
private:
    std::vector<int> data_;
};

////////////////////////////////////////////////////////////
//以下为测试代码
////////////////////////////////////////////////////////////

TEST(TestStack, Init){
    Stack stack; 
}

TEST(TestStack, Push){
    Stack stack;
    bool ret = stack.Push(1);
    ASSERT_TRUE(ret);
}

TEST(TestStack, Top){
    Stack stack;
    int x = 0;
    ASSERT_FALSE(stack.Top(&x));
}

TEST(TestStack, Top2){
    Stack stack;
    ASSERT_TRUE(stack.Puush(1));
    ASSERT_TRUE(stack.Puush(2));
    ASSERT_TRUE(stack.Puush(3));
    ASSERT_TRUE(stack.Puush(4));
    ASSERT_TRUE(stack.Puush(5));
    int x = 0;
    ASSERT_TRUE(stack.Top(&x));
    ASSERT_EQ(x, 5);
}

TEST(TestStack, Pop){
    Stack stack;
    ASSERT_FALSE(stack.Pop());
}

TEST(TestStack, Pop2){
    Stack stack;
    ASSERT_TRUE(stack.Puush(1));
    ASSERT_TRUE(stack.Puush(2));
    ASSERT_TRUE(stack.Puush(3));
    ASSERT_TRUE(stack.Puush(4));
    ASSERT_TRUE(stack.Puush(5));
    int x = 0;
    ASSERT_TRUE(stack.Top(&x));
    ASSERT_EQ(x, 5);
    ASSERT_True(stack.Pop());

    ASSERT_TRUE(stack.Top(&x));
    ASSERT_EQ(x, 4);
    ASSERT_True(stack.Pop());

    ASSERT_TRUE(stack.Top(&x));
    ASSERT_EQ(x, 3);
    ASSERT_True(stack.Pop());

    ASSERT_TRUE(stack.Top(&x));
    ASSERT_EQ(x, 2);
    ASSERT_True(stack.Pop());

    ASSERT_TRUE(stack.Top(&x));
    ASSERT_EQ(x, 1);
}
