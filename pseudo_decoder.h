#pragma once

#include <string>
#include <sstream>

#include "bmpimage.h"
#include "bytes.h"

using std::string;
using std::stringstream;

namespace NImage {
    class PseudoDecoder {
        private:
            const static size_t first = 2;

        private:
            BMPImage image;
            
        private:
            void init();

            uint getLSB(uint mask) const;
        public:
            PseudoDecoder(const string& filename);
            PseudoDecoder(const bytes& data);

            string decodeBitMessage();
    };

    PseudoDecoder::PseudoDecoder(const string& filename) 
        : image(filename) 
    {
        init();
    }
    PseudoDecoder::PseudoDecoder(const bytes& data) 
        : image(data) 
    {
        init();
    }

    size_t next(size_t cur) {
        size_t result = cur + 1;
        while (cur) {
            result++;
            cur >>= 1;
        }
        return result;
    }

    uint PseudoDecoder::getLSB(uint mask) const {
        uint bit = 1;
        for (int i = 0; i < 32; ++i) {
            if (mask & bit) return bit;
            bit += bit;
        }
        return 0;
    }

    void PseudoDecoder::init() {
    }

    string PseudoDecoder::decodeBitMessage() {
        stringstream result;

        bytes data = image.getImageData();
        int width = image.getWidth();
        int height = image.getHeight();
        size_t bytesPerPixel = image.getBitsPerPixel() / 8;
        uint rowAlignment = 4 - ((bytesPerPixel * width) & 3);
        uint rowLength = width * bytesPerPixel + rowAlignment;

        uint redMaskBit = getLSB(image.getRedMask());

        uint bitNo = 0;
        size_t cur = 0;
        size_t need = PseudoDecoder::first;
        for (int row = 0; row < height; ++row) {
            size_t rowOffset = row * rowLength;
            for (int col = 0; col < width; ++col) {
                size_t pixelOffset = rowOffset + col * bytesPerPixel;
                uint pixelData = uintFromBytes(data.begin() + pixelOffset, data.begin() + pixelOffset + bytesPerPixel);
                ++cur;

                if (cur == need) {
                    need = next(cur);
                    uint mask = redMaskBit;

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


