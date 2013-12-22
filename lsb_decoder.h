#pragma once

#include <string>
#include <sstream>

#include "bmpimage.h"
#include "bytes.h"

using std::string;
using std::stringstream;

namespace NImage {
    class LSBDecoder {
        private:
            BMPImage image;
            
        private:
            void init();

            uint getLSB(uint mask) const;
        public:
            LSBDecoder(const string& filename);
            LSBDecoder(const bytes& data);

            string decodeBitMessage();
    };

    LSBDecoder::LSBDecoder(const string& filename) 
        : image(filename) 
    {
        init();
    }
    LSBDecoder::LSBDecoder(const bytes& data) 
        : image(data) 
    {
        init();
    }

    uint LSBDecoder::getLSB(uint mask) const {
        uint bit = 1;
        for (int i = 0; i < 32; ++i) {
            if (mask & bit) return bit;
            bit += bit;
        }
        return 0;
    }

    void LSBDecoder::init() {
    }

    string LSBDecoder::decodeBitMessage() {
        stringstream result;

        bytes data = image.getImageData();
        int width = image.getWidth();
        int height = image.getHeight();
        size_t bytesPerPixel = image.getBitsPerPixel() / 8;
        uint rowAlignment = 4 - ((bytesPerPixel * width) & 3);
        uint rowLength = width * bytesPerPixel + rowAlignment;

        uint redMaskBit = getLSB(image.getRedMask());
        uint greenMaskBit = getLSB(image.getGreenMask());
        uint blueMaskBit = getLSB(image.getBlueMask());
        vector<uint> lsbs(3);
        lsbs[0] = redMaskBit;  
        lsbs[1] = greenMaskBit;  
        lsbs[2] = blueMaskBit;  

        uint bitNo = 0;
        for (int row = 0; row < height; ++row) {
            size_t rowOffset = row * rowLength;
            for (int col = 0; col < width; ++col) {
                size_t pixelOffset = rowOffset + col * bytesPerPixel;
                uint pixelData = uintFromBytes(data.begin() + pixelOffset, data.begin() + pixelOffset + bytesPerPixel);

                for (size_t canalNo = 0; canalNo < lsbs.size(); ++canalNo) {
                    uint mask = lsbs[canalNo];

                    if (pixelData & mask) {
                        result << '1';
                    } else {
                        result << '0';
                    }
                }
            }
        }

        return result.str();
    }
}


