#include <iostream>
#include <sstream>
#include <string>

#include "test.h"

#include "bytes.h"

using namespace std;

class DigitToHexTest : public NTest::Test {
public:
    virtual void test() {
        const char anses[] = "0123456789abcdef";
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
class DumpToHexTest: public NTest::Test {
public:
    virtual void test() {
        NImage::bytes data;
        for (int i = 0; i < 16; ++i) {
            data.push_back(i);
        }
        NTest::checkEqual(NImage::dumpToHex(data), "0001 0203 0405 0607 | 0809 0a0b 0c0d 0e0f\n");

        data.clear();
        for (int i = 0; i < 40; ++i) data.push_back(255);
        
        NTest::checkEqual(NImage::dumpToHex(data), "ffff ffff ffff ffff | ffff ffff ffff ffff\n"
                                                   "ffff ffff ffff ffff | ffff ffff ffff ffff\n"
                                                   "ffff ffff ffff ffff\n");

        data.clear();
        data.push_back(13);
        NTest::checkEqual(NImage::dumpToHex(data), "0d\n");
        data.push_back(48);
        NTest::checkEqual(NImage::dumpToHex(data), "0d30\n");
        data.push_back(255);
        NTest::checkEqual(NImage::dumpToHex(data), "0d30 ff\n");
    }
};
class ToHexTest: public NTest::Test {
public:
    virtual void test() {
        NTest::checkEqual(NImage::toHex(10), "0a"); 
        NTest::checkEqual(NImage::toHex(255), "ff"); 
        NTest::checkEqual(NImage::toHex(0), "00"); 
        NTest::checkEqual(NImage::toHex(16), "10"); 
    }
};
class ReadBytesFromFileTest: public NTest::Test {
public:
    virtual void test() {  
        NImage::bytes result = NImage::readBytesFromFile("test_files/read_bytes.txt");
        NTest::checkEqual("6162 6163 6162 610a | 0a\n", NImage::dumpToHex(result), "method readBytesFromFile doesn't work for file test_files/read_bytes.txt");
    }
};

int main() {
    NTest::TestEnvironment::test(new DigitToHexTest);
    NTest::TestEnvironment::test(new DigitToHexNegativeTest);
    NTest::TestEnvironment::test(new ToHexTest);
    NTest::TestEnvironment::test(new DumpToHexTest);
    NTest::TestEnvironment::test(new ReadBytesFromFileTest);

    return 0;
}

