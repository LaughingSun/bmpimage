#include <string>
#include <vector>
#include <stdexcept>

using std::string;
using std::vector;
using std::runtime_error;

namespace NImage {

class BMPImage {
public:
    typedef unsigned char byte;
    typedef vector<byte> bytes;
private:
    bytes fileHeader;
    bytes pallete;
    bytes imageHeader;
    bytes imageData;
    
    int width, height;
    int bitsPerPixel;

public:
    BMPImage(const string& filename);
    BMPImage(const bytes& data);

    
};

BMPImage::bytes readBytesFromFile(const string& filename) {  
     
}

char digitToHex(BMPImage::byte digit) {
    if (digit >= 16) {
        throw runtime_error("Digit must be in range [0..15]");
    }
    if (digit < 10) return digit + '0';
    return digit - 10 + 'A';
}

string toHex(BMPImage::byte value) {
    return string() + digitToHex(value >> 8) + digitToHex(value & 63);
}

void outputHex(const BMPImage::bytes& data) {
    
}

}
