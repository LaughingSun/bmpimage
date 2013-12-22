#include <iostream>
#include <sstream>
#include <string>

#include "test.h"

#include "bytes.h"
#include "bmpimage.h"

using namespace std;

class DigitToHexTest : public NTest::Test {
    public:
        virtual string getName() {return "digitToHex() test";}

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
        virtual string getName() {return "digitToHex() negative test";}

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
        virtual string getName() {return "dumpToHex() test";}

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
        virtual string getName() {return "toHex() test";}

        virtual void test() {
            NTest::checkEqual(NImage::toHex(10), "0a"); 
            NTest::checkEqual(NImage::toHex(255), "ff"); 
            NTest::checkEqual(NImage::toHex(0), "00"); 
            NTest::checkEqual(NImage::toHex(16), "10"); 
        }
};
class ReadBytesFromFileTest: public NTest::Test {
    public:
        virtual string getName() {return "readBytesFromFile() test";}

        virtual void test() {  
            NImage::bytes result = NImage::readBytesFromFile("test_files/read_bytes.txt");
            NTest::checkEqual("6162 6163 6162 610a | 0a\n", NImage::dumpToHex(result), "method readBytesFromFile doesn't work for file test_files/read_bytes.txt");
        }
};
class UintFromBytesTest: public NTest::Test {
    public:
        virtual string getName() {return "uintFromBytes(const bytes&) test";}

        virtual void test() {
            NImage::bytes data;
            NTest::checkEqual(NImage::uintFromBytes(data), NImage::uint(0), "uint from {} must be equal to zero");
            NTest::checkEqual(NImage::uintFromBytes(data.begin(), data.end()), NImage::uint(0), "uint from {} must be equal to zero");
            data.push_back(1);
            NTest::checkEqual(NImage::uintFromBytes(data), NImage::uint(1), "uint from {1} must be equal to 1");
            NTest::checkEqual(NImage::uintFromBytes(data.begin(), data.end()), NImage::uint(1), "uint from {1} must be equal to 1");
            data.push_back(2);
            NTest::checkEqual(NImage::uintFromBytes(data), NImage::uint(513), "uint from {1, 2} must be equal to 513");
            NTest::checkEqual(NImage::uintFromBytes(data.begin(), data.end()), NImage::uint(513), "uint from {1, 2} must be equal to 513");

            data.push_back(3);
            NTest::checkEqual(NImage::uintFromBytes(data), NImage::uint(197121), "uint from {1, 2, 3} must be equal to 197121");
            NTest::checkEqual(NImage::uintFromBytes(data.begin(), data.end()), NImage::uint(197121), "uint from {1, 2, 3} must be equal to 197121");
            data.push_back(3);
            NTest::checkEqual(NImage::uintFromBytes(data), NImage::uint(50528769), "uint from {1, 2, 3, 3} must be equal to 50528769");
            NTest::checkEqual(NImage::uintFromBytes(data.begin(), data.end()), NImage::uint(50528769), "uint from {1, 2, 3, 3} must be equal to 50528769");
            data.push_back(3);
            
            try {
                NImage::uintFromBytes(data); 
            } catch (const runtime_error& e) {
                NTest::log("Too big bytes array test passed (vector)"); 
            }
            try {
                NImage::uintFromBytes(data.begin(), data.end()); 
            } catch (const runtime_error& e) {
                NTest::log("Too big bytes array test passed (iterators)"); 
            }
        }
};
class BMPImageTest: public NTest::Test {
    public:
        virtual string getName() {return "BMPImage class test";}

        virtual void test() {
            using NImage::uint;

            try {
                NImage::BMPImage image("test_files/8-8-8_false_signature.bmp"); 
            } catch (const runtime_error& e) {
                NTest::log("False signature test passed");
            }

            NImage::BMPImage image("test_files/8-8-8.bmp");
            NTest::checkEqual(image.getOffsetToData(), uint(70), "getOffsetToData() test is failed");
            NTest::checkEqual(image.getImageHeaderSize(), uint(40), "getImageHeaderSize() test is failed");
            NTest::checkEqual(image.getCompressionType(), uint(0), "getCompressionType() test is failed");
            NTest::checkEqual(image.getHeight(), uint(3), "getHeight() test is failed");
            NTest::checkEqual(image.getWidth(), uint(3), "getWidth() test is failed");
            NTest::checkEqual(image.getBitsPerPixel(), uint(8), "getBitsPerPixel() test is failed");
            
            NImage::BMPImage image2("test_files/X8-8-8-8.bmp");
            NTest::checkEqual(image2.getOffsetToData(), uint(70), "getOffsetToData() test is failed");
            NTest::checkEqual(image2.getImageHeaderSize(), uint(56), "getImageHeaderSize() test is failed");
            NTest::checkEqual(image2.getCompressionType(), uint(3), "getCompressionType() test is failed");
            NTest::checkEqual(image2.getHeight(), uint(2), "getHeight() test is failed");
            NTest::checkEqual(image2.getWidth(), uint(2), "getWidth() test is failed");
            NTest::checkEqual(image2.getBitsPerPixel(), uint(32), "getBitsPerPixel() test is failed");
            NTest::checkEqual(image2.getRedMask(), uint(4278190080), "getRedMask() test is failed");
            NTest::checkEqual(image2.getGreenMask(), uint(16711680), "getGreenMask() test is failed");
            NTest::checkEqual(image2.getBlueMask(), uint(65280), "getBlueMask() test is failed");
        }
};

int main() {
    NTest::TestEnvironment::addTest(new DigitToHexTest);
    NTest::TestEnvironment::addTest(new DigitToHexNegativeTest);
    NTest::TestEnvironment::addTest(new ToHexTest);
    NTest::TestEnvironment::addTest(new DumpToHexTest);
    NTest::TestEnvironment::addTest(new ReadBytesFromFileTest);
    NTest::TestEnvironment::addTest(new BMPImageTest);
    NTest::TestEnvironment::addTest(new UintFromBytesTest);

    NTest::TestEnvironment::run();

    return 0;
}

