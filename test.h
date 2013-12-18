#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <stdlib.h>

using namespace std;

namespace NTest {
#ifdef WIN32
    #define KNRM
    #define KRED
    #define KGRN
#else
    #define KNRM  "\x1B[0m"
    #define KRED  "\x1B[31m"
    #define KGRN  "\x1B[32m"
#endif
#define COLORED(txt, color) color txt KNRM

    enum STATUS_CODE {
        OK = 0,
        ERROR = 1
    };
    STATUS_CODE status = OK;
    vector<string> errorMessages;

    void error(const string& message) {
        status = ERROR;
        errorMessages.push_back(message);
    }

    void checkTrue(bool condition, const string& message) {
        if (!condition) {
            error(message);
        }
    }

    template<typename T>
    void checkEqual(const T& a, const T& b, const string& message) {
        checkTrue(a == b, message);
    }

    template<typename T>
    void checkEqual(const T& a, const T& b) {
        stringstream ss;
        ss << a << " is not equal to " << b;
        checkEqual(a, b, ss.str());
    }

    void checkTrue(bool condition) {
        checkTrue(condition, "Condition is not hold");
    }

    void checkEqual(const string& a, const char* b) {
        checkEqual(a, string(b));
    }
    void checkEqual(const char* a, const string& b) {
        checkEqual(string(a), b);
    }
    void checkEqual(const string& a, const char* b, const string& message) {
        checkEqual(a, string(b), message);
    }
    void checkEqual(const char* a, const string& b, const string& message) {
        checkEqual(string(a), b, message);
    }

    class Test {
    public:
        virtual void setUp() {};
        virtual void test() = 0;
        virtual void tearDown() {};

        virtual ~Test() {};
    };

    class TestEnvironment {
    private:
        static size_t testNo;

    public:
        static bool test(Test* test) {
            if (status != OK) {
                cerr << "Test already failed!" << endl;
                return false;
            }

            ++testNo;
            cout << "Test #" << testNo << "\t";

            test->setUp();
            try {
                test->test();
            } catch (...) {
                error("Exception has been caugth"); 
            }
            test->tearDown();
           
            if (status == OK) {
                cout << COLORED("PASSED", KGRN) << endl;
                return true;
            }

            cout << COLORED("FAILED", KRED) << endl;
            for (size_t i = 0; i < errorMessages.size(); ++i) {
                cout << "\t" << errorMessages[i] << endl;
            }
            errorMessages.clear();
            exit(1);
            return false;
        }
    };
    size_t TestEnvironment::testNo = 0;
    
    
#undef COLORED
#undef KGRN
#undef KRED
#undef KNRM
};

