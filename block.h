#pragma once

#include <string>
#include <sstream>

#include "bmpimage.h"
#include "bytes.h"

using std::string;
using std::stringstream;

namespace NImage {
    class BlockEncoder {
        private:
            BMPImage image;
            
            uint maximumLength;
        private:
            void init();

            uint getLSB(uint mask) const;
        public:
            BlockEncoder(const string& filename);
            BlockEncoder(const bytes& data);

            uint getMaximumLength() const;

            void encodeBitMessage(const string& message);
            void writeMessageToFile(const string& filename);
    };

    BlockEncoder::BlockEncoder(const string& filename) 
        : image(filename) 
    {
        init();
    }
    BlockEncoder::BlockEncoder(const bytes& data) 
        : image(data) 
    {
        init();
    }

    uint BlockEncoder::getLSB(uint mask) const {
        uint bit = 1;
        for (int i = 0; i < 32; ++i) {
            if (mask & bit) return bit;
            bit += bit;
        }
        return 0;
    }

    void BlockEncoder::init() {
        maximumLength = 0;
        if (image.getCompressionType() == 3 && image.getBitsPerPixel() >= 16) {
            maximumLength = (image.getWidth() / 3) * image.getHeight();
        }
    }

    uint BlockEncoder::getMaximumLength() const {
        return maximumLength;
    }

    void BlockEncoder::encodeBitMessage(const string& tmessage) {
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

        uint bitNo = 0;
        for (int row = 0; row < height; ++row) {
            size_t rowOffset = row * rowLength;
            for (int col = 0; col < width; col += 3) {
                size_t pixelOffsets[3];
                uint pixelDatas[3];
                bool cur = false;
                for (int i = 0; i < 3; ++i) {
                    pixelOffsets[i] = rowOffset + (col + i) * bytesPerPixel;
                    pixelDatas[i] = uintFromBytes(data.begin() + pixelOffsets[i], data.begin() + pixelOffsets[i] + bytesPerPixel);
                    
                    if (pixelDatas[i] & redMaskBit) {
                        cur ^= true;
                    }
                }

                bool bit = message[bitNo++] == '1';
                if (cur != bit) {
                    pixelDatas[1] ^= redMaskBit;
                }

                for (int i = 0; i < 3; ++i) {
                    bytesFromUint(data.begin() + pixelOffsets[i], data.begin() + pixelOffsets[i] + bytesPerPixel, pixelDatas[i]);
                }
            }
        }

        image.setImageData(data);
    }

    void BlockEncoder::writeMessageToFile(const string& filename) {
        writeBytesToFile(filename, image.getFullData());
    }
}


