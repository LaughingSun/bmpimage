#pragma once

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

    string colored(const string& txt, const string& color) {
        stringstream ss;
        ss << color << txt << KNRM;
        return ss.str();
    }

    enum STATUS_CODE {
        OK = 0,
        ERROR = 1
    };
    STATUS_CODE status = OK;

    void log(const string& message) {
        cout << colored("\t" + message, KGRN) << endl;
    }

    void error(const string& message) {
        status = ERROR;
        cout << colored("\t" + message, KRED) << endl; 
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
        virtual string getName() {return "Unnamed test";}

        virtual ~Test() {};
    };

    class TestEnvironment {
    private:
        static size_t testNo;
        static vector< Test* > tests;

        static bool test(Test* test) {
            status = OK;

            test->setUp();
            try {
                test->test();
            } catch (const exception& e) {
                error("Exception has been caught");
                error(string("\t") + e.what()); 
            } catch (...) {
                error("Exception has been caugth"); 
            }
            test->tearDown();
           
            if (status == OK) {
                return true;
            }

            return false;
        }

        static string getAligned(const string& message, int width) {
            string result = message;
            while (result.size() < width) {
                result += ' ';
            }
            return result;
        }

        static string getXeroxed(string pattern, int times) {
            string result;
            while (times--) {
                result += pattern;
            }
            return result;
        }
    public:
        static void addTest(Test* test) {
            tests.push_back(test); 
        }

        static void run() {
            size_t all = tests.size();
            size_t passed = 0;

            vector<bool> isPassed(tests.size());
            for (size_t i = 0; i < tests.size(); ++i) {
                cout << "Running test #" << i + 1 << " (" << tests[i]->getName() << ")" << endl;
                isPassed[i] = test(tests[i]);
                if (isPassed[i]) {
                    ++passed;
                    cout << colored("Test is passed\n", KGRN);
                } else {
                    cout << colored("Test is failed\n", KRED);
                }
            }

            const int NUMBER_WIDTH = 5;
            const int NAME_WIDTH = 40;
            const int RESULT_WIDTH = 10;
            const int ROW_WIDTH = NUMBER_WIDTH + NAME_WIDTH + RESULT_WIDTH + 7; // two seprators

            cout << "\n\n\n";
            cout << getXeroxed("-", ROW_WIDTH) << endl;
            cout << getAligned("| #", NUMBER_WIDTH) << " | ";
            cout << getAligned("name", NAME_WIDTH) << " | ";
            cout << getAligned("result", RESULT_WIDTH) << "|\n";
            cout << getXeroxed("-", ROW_WIDTH) << endl;

            for (int i = 0; i < tests.size(); ++i) {
                stringstream ss;
                ss << "| " << i;
                cout << getAligned(ss.str(), NUMBER_WIDTH) << " | ";
                cout << getAligned(tests[i]->getName(), NAME_WIDTH) << " | ";
                if (isPassed[i]) {
                    cout << colored(getAligned("PASSED", RESULT_WIDTH), KGRN) << "|" << endl;
                } else {
                    cout << colored(getAligned("FAILED", RESULT_WIDTH), KRED) << "|" << endl;
                }
            }
            cout << getXeroxed("-", ROW_WIDTH) << endl << endl;

            if (all == passed) {
                cout << colored("All tests passed", KGRN) << endl;
            } else {
                stringstream ss;
                ss << (all - passed) << " out of " << all << " tests failed";
                cout << colored(ss.str(), KRED) << endl;
            }
        }

        
    };
    size_t TestEnvironment::testNo = 0;
    vector<Test*> TestEnvironment::tests = vector<Test*>();
    
    
#undef COLORED
#undef KGRN
#undef KRED
#undef KNRM
};

