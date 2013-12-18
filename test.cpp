#include <iostream>

#include "test.h"

using namespace std;

class StupidPlusTestOK : public NTest::Test {
public:
    virtual void test() {
        NTest::checkEqual(2 + 2, 4);
    }
};
class StupidPlusTestWA : public NTest::Test {
public:
    virtual void test() {
        NTest::checkEqual(2 + 2, 3);
    }
};

int main() {
    NTest::TestEnvironment::test(new StupidPlusTestOK);
    NTest::TestEnvironment::test(new StupidPlusTestOK);
    NTest::TestEnvironment::test(new StupidPlusTestOK);
    NTest::TestEnvironment::test(new StupidPlusTestWA);
    NTest::TestEnvironment::test(new StupidPlusTestOK);
    NTest::TestEnvironment::test(new StupidPlusTestOK);

    return 0;
}

