#pragma once

#include <string>
#include <sstream>

#include "bmpimage.h"
#include "bytes.h"

using std::string;
using std::stringstream;

namespace NImage {
    class BlockDecoder {
        private:
            BMPImage image;
            
        private:
            void init();

            uint getLSB(uint mask) const;
        public:
            BlockDecoder(const string& filename);
            BlockDecoder(const bytes& data);

            string decodeBitMessage();
    };

    BlockDecoder::BlockDecoder(const string& filename) 
        : image(filename) 
    {
        init();
    }
    BlockDecoder::BlockDecoder(const bytes& data) 
        : image(data) 
    {
        init();
    }

    uint BlockDecoder::getLSB(uint mask) const {
        uint bit = 1;
        for (int i = 0; i < 32; ++i) {
            if (mask & bit) return bit;
            bit += bit;
        }
        return 0;
    }

    void BlockDecoder::init() {
    }

    string BlockDecoder::decodeBitMessage() {
        stringstream result;

        bytes data = image.getImageData();
        int width = image.getWidth();
        int height = image.getHeight();
        size_t bytesPerPixel = image.getBitsPerPixel() / 8;
        uint rowAlignment = 4 - ((bytesPerPixel * width) & 3);
        uint rowLength = width * bytesPerPixel + rowAlignment;

        uint redMaskBit = getLSB(image.getRedMask());

        for (int row = 0; row < height; ++row) {
            size_t rowOffset = row * rowLength;
            for (int col = 0; col < width; col += 3) {
                bool value = false;
                for (int i = 0; i < 3; ++i) {
                    size_t pixelOffset = rowOffset + (col + i) * bytesPerPixel;
                    uint pixelData = uintFromBytes(data.begin() + pixelOffset, data.begin() + pixelOffset + bytesPerPixel);

                    if (pixelData & redMaskBit) value ^= true;
                }

                if (value) result << '1';
                else result << '0';
            }
        }

        return result.str();
    }
}


