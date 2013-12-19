#pragma once

#include <stdexcept>

#include "bytes.h"

using std::runtime_error;

namespace NImage {
    class BMPImage {
        private:
            bytes fileHeader;
            bytes pallete;
            bytes imageHeader;
            bytes imageData;

            int width, height;
            int bitsPerPixel;

        private:
            void init(const bytes& data);

        public:
            BMPImage(const string& filename);
            BMPImage(const bytes& data);
    };

    void BMPImage::init(const bytes& data) {
        if (data.size() < 10) {
            throw runtime_error("Bytes array is too small to represent bmp"); 
        }
        if (data[0] != 'B' || data[1] != 'M') {
            throw runtime_error("Signature is invalide. First two bytes must be BM");
        }


    }

    BMPImage::BMPImage(const bytes& data) {
        init(data);
    }

    BMPImage::BMPImage(const string& filename) {
        init(readBytesFromFile(filename)); 
    }
}

