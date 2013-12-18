#include <string>
#include <vector>

using std::string;
using std::vector;

class BMPImage {
private:
    typedef unsigned char byte;
    typedef vector<byte> bytes;
private:
    bytes fileHeader;
    bytes imageHeader;
    bytes imageData;
    
    int width, height;
    int bitsPerPixel;
public:
    BMPImage(const string& filename);
    BMPImage(const bytes& data);
};
