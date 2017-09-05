//
//  Aluminium.hpp
//
//  Created by Michael Chiou on 15/07/2017.
//  MIT License
//  Copyright (c) 2017
//
#ifndef __ALUMINIUM_H__
#define __ALUMINIUM_H__

#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>

//
//ASSERTIONS
//

#define ASSERT(_condition)\
    ASSERT_TRUE(_condition);

#define ASSERT_TRUE(condition_)\
    if ( !(condition_) )\
    {\
        throw aluminium::AssertFailedException(#condition_, __FILE__, __LINE__);\
    }

#define ASSERT_FALSE(condition_)\
    if (condition_)\
    {\
        throw aluminium::AssertFailedException(#condition_, __FILE__, __LINE__);\
    }

#define ASSERT_NULL(_value)\
    ASSERT_TRUE(_value == nullptr);

#define ASSERT_NOTNULL(_value)\
    ASSERT_TRUE(_value != nullptr);

#define ASSERT_STREQ(a, b)\
    if (std::string(a).compare(std::string(b)) != 0)\
    {\
        aluminium::PrintTestInfo(a, b, " != ");\
        throw aluminium::AssertFailedException(#a " == " #b, __FILE__, __LINE__);\
    }

#define ASSERT_STRNEQ(a, b)\
    if (std::string(a).compare(std::string(b)) == 0)\
    {\
        aluminium::PrintTestInfo(a, b, " == ");\
        throw aluminium::AssertFailedException(#a " != " #b, __FILE__, __LINE__);\
    }

#define ASSERT_EQ(a, b)\
    if (a != b) \
    {\
        aluminium::PrintTestInfo(a, b, " != ");\
    }\
    ASSERT(a == b);

#define ASSERT_NEQ(a, b)\
    if (a == b) \
    {\
        aluminium::PrintTestInfo(a, b, " == ");\
    }\
    ASSERT(a != b);

#define ASSERT_GT(a, b)\
    if (a < b) \
    {\
        aluminium::PrintTestInfo(a, b, " > ");\
    }\
    ASSERT(a > b);

#define ASSERT_LT(a, b)\
    if (a > b) \
    {\
        aluminium::PrintTestInfo(a, b, " < ");\
    }\
    ASSERT(a < b);

#define ASSERT_GE(a, b)\
    if (a < b) \
    {\
        aluminium::PrintTestInfo(a, b, " => ");\
    }\
    ASSERT(a >= b);

#define ASSERT_LE(a, b)\
    if (a > b) \
    {\
        aluminium::PrintTestInfo(a, b, " <= ");\
    }\
    ASSERT(a <= b);

//
//Unit Tests
//

#define TEST(case, name)\
void name();\
void case();\
namespace\
{\
    bool __##name = aluminium::TestsManager::AddTest(name, #case, #name);\
}\
void name()

//
// Framework
//

namespace aluminium
{
    inline void PrintCase(const std::string& case_name_)
    {
        std::cout << "Case : " << case_name_ << std::endl;
    }

    inline void PrintFailCase(const std::string& case_name_, size_t failed_tests_)
    {
        std::cout << "Case : " << case_name_ << " FAILED" << std::endl << std::endl;
    }

    inline void PrintPassCase(const std::string& case_name_)
    {
        std::cout << "Case : " << case_name_ << " PASSED" << std::endl << std::endl;
    }

    inline void PrintRunning(const std::string& case_name_, const std::string& test_name_)
    {
        std::cout << "    [ running] " << case_name_ << "::" << test_name_ << std::endl;
    }

    inline void PrintPass(const std::string& case_name_, const std::string& test_name_)
    {
        std::cout << "    [  PASSED] " << case_name_ << "::" << test_name_ << std::endl;
    }

    inline void PrintFailed(const std::string& case_name_, const std::string& test_name_)
    {
        std::cout << "    [  FAILED] " << case_name_ << "::" << test_name_ << std::endl;
    }

    inline void PrintExpetionInfo(const std::string& exception_what_, const std::string& file_path_)
    {
        std::cout << "               Assertion failed: " << exception_what_ << std::endl;
        std::cout << "               " << file_path_ << " : " << exception_what_ << std::endl;
    }

    template<typename Type>
    inline void PrintTestInfo(const Type& a, const Type& b, const std::string& opperator_)
    {
        std::cout << "    [    info] " << std::endl;\
        std::cout << "               Actual values: " << a << opperator_ << b << std::endl;\
    }

    // Exception that is thrown when an assertion fails.
    class AssertFailedException : public std::exception
    {
    public:
        AssertFailedException(std::string description, std::string filepath, int line) :
        std::exception(),
        __description(description),
        __filepath(filepath),
        __line(line)
        {}
        inline const std::string& GetDescription(void) const throw() { return __description; }
        inline const std::string& GetFilePath(void) { return __filepath; }
        inline unsigned int GetLine(void) { return __line; }
    protected:
        std::string __description;
        std::string __filepath;
        int __line;
    };

    class TestsManager
    {
    public:

        struct Test
        {
            Test( const std::function<void(void)>& function_, const std::string& name_)
            {
                __function = function_;
                __name = name_;
            }
            std::string __name;
            std::function<void(void)> __function;
        };

        struct Case
        {
            Case(const std::string& name_)
            {
                __name = name_;
            }
            void AddTest(const Test& test_)
            {
                __tests.push_back(test_);
            }
            std::vector<Test> __tests;
            std::string __name;
        };

        static std::vector<Case>& Cases()
        {
            static std::vector<Case> __cases;
            return __cases;
        }

        inline static size_t Count(void)
        {
            static size_t count{0};
            for(const auto& c : Cases())
            {
                count+= c.__tests.size();
            }
            return count;
        }

        // Adds a new test to the current case of tests.
        inline static bool AddTest(std::function<void(void)> function_, std::string case_name_, std::string test_name_)
        {
            auto comperator( [&](const Case& case_) { return case_.__name == case_name_; } );
            auto match( find_if( Cases().begin(), Cases().end(), comperator) );

            if(match != Cases().cend())
            {
                (*match).AddTest(Test(function_, test_name_));
                return true;
            }
            Case c(case_name_);
            c.__tests.push_back(Test(function_, test_name_));
            Cases().push_back(c);
            return true;
        }

        inline static size_t ExecuteTest(const std::string& case_name_, const std::string& test_name_, std::function<void(void)> function_)
        {
            try
            {
                PrintRunning(case_name_, test_name_);
                function_();
                PrintPass(case_name_, test_name_);
                return 0;
            }
            catch (AssertFailedException& exp)
            {
                PrintFailed(case_name_, test_name_);
                PrintExpetionInfo(exp.GetDescription(), exp.GetFilePath());
                return 1;
            }
        }

        // Run all tests that are registered.
        // Returns the number of tests that failed.
        inline static size_t ExecuteAllTests()
        {
            size_t overall_failed{0};

            for (const auto& _case : Cases())
            {
                PrintCase(_case.__name);
                size_t specific_failed{0};
                for(const auto& test : _case.__tests)
                {
                    // Run the test.
                    // If an AsserFailedException is thrown, the test has failed.
                    auto result( ExecuteTest(_case.__name, test.__name, test.__function) );
                    overall_failed += result;
                    specific_failed += result;
                }
                (specific_failed > 0) ? PrintFailCase(_case.__name, specific_failed) : PrintPassCase(_case.__name);
            }
            return overall_failed;
        }
    };
}

#define RUN_TESTS()\
    size_t failed_tests{aluminium::TestsManager::ExecuteAllTests()};\
    if (failed_tests == 0)\
    {\
        std::cout << "    [ summary] All tests succeeded!" << std::endl;\
    }\
    else\
    {\
        std::cerr << "    [ summary] " << failed_tests << " tests failed out of " << aluminium::TestsManager::Count() << std::endl;\
    }\

#endif // __ALUMINIUM_H__
