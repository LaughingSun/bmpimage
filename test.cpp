#include <iostream>
#include <sstream>
#include <string>

#include "test.h"

#include "bmpimage.h"

using namespace std;

class DigitToHexTest : public NTest::Test {
public:
    virtual void test() {
        const char anses[] = "0123456789ABCDEF";
        for (int i = 0; i < 16; ++i) {
            char ans = NImage::digitToHex(i);
            stringstream ss;
            ss << "Hex representation for " << i << " must be " << anses[i]<< ", not " << ans;
            NTest::checkEqual(ans, anses[i], ss.str());
        }
    }
};

int main() {
    NTest::TestEnvironment::test(new DigitToHexTest);

    return 0;
}

