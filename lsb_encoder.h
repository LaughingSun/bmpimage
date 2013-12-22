#pragma once

#include <string>
#include <sstream>

#include "bmpimage.h"
#include "bytes.h"

using std::string;
using std::stringstream;

namespace NImage {
    class LSBEncoder {
        private:
            BMPImage image;
            
            uint maximumLength;
        private:
            void init();

            uint getLSB(uint mask) const;
        public:
            LSBEncoder(const string& filename);
            LSBEncoder(const bytes& data);

            uint getMaximumLength() const;

            void encodeBitMessage(const string& message);
            void writeMessageToFile(const string& filename);
    };

    LSBEncoder::LSBEncoder(const string& filename) 
        : image(filename) 
    {
        init();
    }
    LSBEncoder::LSBEncoder(const bytes& data) 
        : image(data) 
    {
        init();
    }

    uint LSBEncoder::getLSB(uint mask) const {
        uint bit = 1;
        for (int i = 0; i < 32; ++i) {
            if (mask & bit) return bit;
            bit += bit;
        }
        return 0;
    }

    void LSBEncoder::init() {
        maximumLength = 0;
        if (image.getCompressionType() == 3 && image.getBitsPerPixel() >= 16) {
            maximumLength = image.getWidth() * image.getHeight() * 3;
        }
    }

    uint LSBEncoder::getMaximumLength() const {
        return maximumLength;
    }

    void LSBEncoder::encodeBitMessage(const string& tmessage) {
        string message = tmessage;
        if (message.size() > maximumLength) {
            stringstream ss;
            ss << "Message is to big (" << message.size() << " bits), can only encode " << maximumLength << " bits";
            throw runtime_error(ss.str());
        }
        for (size_t i = 0; i < message.size(); ++i) {
            if (message[i] != '0' && message[i] != '1') {
                throw runtime_error("Message must contain only ones and zeros");
            }
        }
        while (message.size() < maximumLength) {
            message += '0';
        }

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
                    uint bit = message[bitNo++] - '0';
                    uint mask = lsbs[canalNo];

                    if (bit) {
                        pixelData |= mask;
                    } else {
                        if (pixelData & mask) {
                            pixelData ^= mask;
                        }
                    }
                }

                bytesFromUint(data.begin() + pixelOffset, data.begin() + pixelOffset + bytesPerPixel, pixelData);
            }
        }

        image.setImageData(data);
    }

    void LSBEncoder::writeMessageToFile(const string& filename) {
        writeBytesToFile(filename, image.getFullData());
    }
}


