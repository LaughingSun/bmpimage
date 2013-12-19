#include "bytes.h"

namespace NImage {
    class BMPImage {
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


}
