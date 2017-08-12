# Aluminium
Single Header C++ Unit Testing Framework

**Aluminium** is a testing framework for **C++**. It is minimal, light, easy to use and it comprises of a single header file.
You can create test cases, which will allow you to better organize your unit tests. Its simplicity allows for fast and easy use. Just include the header file in your project and you are good to go.

## Requirements

Aluminium is written using the C++14 standard.

## How to Use

```cpp
#include <Aluminium.hpp>

TEST(ExampleCase, ExampleTestPass)
{
    ASSERT_TRUE(true);

    ASSERT_FALSE(false);

    ASSERT_EQ(1, 1);

    ASSERT_NEQ(2, 1);

    ASSERT_GT(2, 1);

    ASSERT_LT(1, 2);

    ASSERT_GE(2, 2);
    ASSERT_GE(2, 1);

    ASSERT_LE(1, 2);
    ASSERT_LE(2, 2);
    
    ASSERT_NULL(nullptr);

    int* _ptr{new int};
    ASSERT_NOTNULL(_ptr);
    delete _ptr;

    ASSERT_STREQ("abc", "abc");

TEST(ExampleCase, ExampleTestFail)
{
    ASSERT_STRNEQ("abc", "abc");
}

int main(int argc, const char * argv[])
{
    RUN_TESTS();
    return 0;
}

```
