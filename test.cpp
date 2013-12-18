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
class DigitToHexNegativeTest : public NTest::Test {
public:
    virtual void test() {
        size_t exceptionCaught = 0;
        for (int i = 0; i < 255; ++i) {
            string result;
            stringstream ss;
            try {
                result = NImage::digitToHex(i);
            } catch (const runtime_error& e) {
                if (i < 16) {
                    ss << i << " actually has hex-representation, but exception has been caught";
                    NTest::error(ss.str()); 
                }
                continue;
            }
            ss << i << " has not hex-representation, but got " << result;  
        }
    }
};
class ToHexTest: public NTest::Test {
public:
    virtual void test() {
        NTest::checkEqual(NImage::toHex(10), "0A"); 
        NTest::checkEqual(NImage::toHex(255), "FF"); 
        NTest::checkEqual(NImage::toHex(0), "00"); 
        NTest::checkEqual(NImage::toHex(16), "10"); 
    }
};

int main() {
    NTest::TestEnvironment::test(new DigitToHexTest);
    NTest::TestEnvironment::test(new DigitToHexNegativeTest);
    NTest::TestEnvironment::test(new ToHexTest);

    return 0;
}

