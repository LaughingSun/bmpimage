#include <string>
#include <vector>

using std::string;
using std::vector;

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

BMPImage::bytes read(const string& filename) {  

}



string toHex(byte value) {
    string 
}

void outputHex(const bytes& data) {
    
}

}
