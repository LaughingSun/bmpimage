#pragma once

#include <string>
#include <sstream>

#include "bmpimage.h"
#include "bytes.h"

using std::string;
using std::stringstream;

namespace NImage {
    class PseudoEncoder {
        private:
            const static size_t first = 2;

        private:
            BMPImage image;
            
            uint maximumLength;
        private:
            void init();

            uint getLSB(uint mask) const;
        public:
            PseudoEncoder(const string& filename);
            PseudoEncoder(const bytes& data);

            uint getMaximumLength() const;

            void encodeBitMessage(const string& message);
            void writeMessageToFile(const string& filename);
    };

    size_t next(size_t cur) {
        size_t result = cur + 1;
        while (cur) {
            result++;
            cur >>= 1;
        }
        return result;
    }

    PseudoEncoder::PseudoEncoder(const string& filename) 
        : image(filename) 
    {
        init();
    }
    PseudoEncoder::PseudoEncoder(const bytes& data) 
        : image(data) 
    {
        init();
    }

    uint PseudoEncoder::getLSB(uint mask) const {
        uint bit = 1;
        for (int i = 0; i < 32; ++i) {
            if (mask & bit) return bit;
            bit += bit;
        }
        return 0;
    }

    void PseudoEncoder::init() {
        maximumLength = 0;
        if (image.getCompressionType() == 3 && image.getBitsPerPixel() >= 16) {
            size_t lim = image.getWidth() * image.getHeight();
            size_t cur = PseudoEncoder::first;
            while (cur < lim) {
                ++maximumLength;
                cur = next(cur);
            }
        }
    }

    uint PseudoEncoder::getMaximumLength() const {
        return maximumLength;
    }

    void PseudoEncoder::encodeBitMessage(const string& tmessage) {
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
        size_t cur = 0;
        size_t need = PseudoEncoder::first;
        for (int row = 0; row < height; ++row) {
            size_t rowOffset = row * rowLength;
            for (int col = 0; col < width; ++col) {
                size_t pixelOffset = rowOffset + col * bytesPerPixel;
                uint pixelData = uintFromBytes(data.begin() + pixelOffset, data.begin() + pixelOffset + bytesPerPixel);
                ++cur;

                if (cur == need) {
                    need = next(need);
                    uint bit = message[bitNo++] - '0';
                    uint mask = redMaskBit; 

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

    void PseudoEncoder::writeMessageToFile(const string& filename) {
        writeBytesToFile(filename, image.getFullData());
    }
}


