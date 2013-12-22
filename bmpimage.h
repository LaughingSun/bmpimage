#pragma once

#include <stdexcept>

#include "bytes.h"

using std::runtime_error;

namespace NImage {
    class BMPImage {
        private:
            const static size_t CT_MASKS = 3;

        private:
            bytes fileHeader;
            bytes pallete;
            bytes imageHeader;
            bytes imageData;

            uint width, height;
            uint bitsPerPixel;
            
            uint offsetToData;
            uint compressionType;

            uint redMask, blueMask, greenMask;
        private:
            void clear();
            void init(const bytes& data);

            void parseImageHeader();

        public:
            BMPImage(const string& filename);
            BMPImage(const bytes& data);

            uint getOffsetToData() const;
            uint getImageHeaderSize() const;
            uint getCompressionType() const;
            uint getWidth() const;
            uint getHeight() const;
            uint getBitsPerPixel() const;
            uint getGreenMask() const;
            uint getBlueMask() const;
            uint getRedMask() const;
            bytes getImageData() const;

            void setImageData(const bytes& newImageData);
            bytes getFullData() const;
    };

    bytes BMPImage::getFullData() const {
        bytes result;
        result.insert(result.end(), fileHeader.begin(), fileHeader.end());
        result.insert(result.end(), imageHeader.begin(), imageHeader.end());
        result.insert(result.end(), pallete.begin(), pallete.end());
        result.insert(result.end(), imageData.begin(), imageData.end());
        return result;
    }

    void BMPImage::clear() {
        fileHeader.clear();         
        pallete.clear();
        imageHeader.clear();
        imageData.clear();

        width = height = 0;
        bitsPerPixel = 0;
        compressionType = 0;
        redMask = blueMask = greenMask = 0;
    }

    uint BMPImage::getImageHeaderSize() const {
        return imageHeader.size();
    }

    uint BMPImage::getOffsetToData() const {
        return offsetToData;
    }

    uint BMPImage::getCompressionType() const {
        return compressionType;
    }

    uint BMPImage::getWidth() const {
        return width;
    }

    uint BMPImage::getHeight() const {
        return height;
    }
    uint BMPImage::getBitsPerPixel() const {
        return bitsPerPixel;
    }

    uint BMPImage::getGreenMask() const {
        return greenMask;
    }
    uint BMPImage::getBlueMask() const {
        return blueMask;
    }
    uint BMPImage::getRedMask() const {
        return redMask;
    }

    bytes BMPImage::getImageData() const {
        return imageData;
    }
    void BMPImage::setImageData(const bytes& newImageData) {
        imageData = newImageData; 
    }

    void BMPImage::parseImageHeader() {
        const size_t COMPRESSION_TYPE_OFFSET = 16;
        const size_t COMPRESSION_TYPE_SIZE = 4;
        const size_t WIDTH_OFFSET = 4;
        const size_t WIDTH_SIZE = 4;
        const size_t HEIGHT_OFFSET = 8;
        const size_t HEIGHT_SIZE = 4;
        const size_t BITS_PER_PIXEL_OFFSET = 14;
        const size_t BITS_PER_PIXEL_SIZE = 2;

        const size_t RED_MASK_OFFSET = 40;
        const size_t GREEN_MASK_OFFSET = 44;
        const size_t BLUE_MASK_OFFSET = 48;
        const size_t MASK_SIZE = 4;

        compressionType = uintFromBytes(imageHeader.begin() + COMPRESSION_TYPE_OFFSET, imageHeader.begin() + COMPRESSION_TYPE_OFFSET + COMPRESSION_TYPE_SIZE);  
        width = uintFromBytes(imageHeader.begin() + WIDTH_OFFSET, imageHeader.begin() + WIDTH_OFFSET + WIDTH_SIZE);
        height = uintFromBytes(imageHeader.begin() + HEIGHT_OFFSET, imageHeader.begin() + HEIGHT_OFFSET + HEIGHT_SIZE);
        bitsPerPixel = uintFromBytes(imageHeader.begin() + BITS_PER_PIXEL_OFFSET, imageHeader.begin() + BITS_PER_PIXEL_OFFSET + BITS_PER_PIXEL_SIZE);

        if (compressionType == CT_MASKS) {
            redMask = uintFromBytes(imageHeader.begin() + RED_MASK_OFFSET, imageHeader.begin() + RED_MASK_OFFSET + MASK_SIZE); 
            greenMask = uintFromBytes(imageHeader.begin() + GREEN_MASK_OFFSET, imageHeader.begin() + GREEN_MASK_OFFSET + MASK_SIZE); 
            blueMask = uintFromBytes(imageHeader.begin() + BLUE_MASK_OFFSET, imageHeader.begin() + BLUE_MASK_OFFSET + MASK_SIZE); 
        }
    }

    void BMPImage::init(const bytes& data) {
        const size_t MIN_HEADERS_SIZE = 22;
        const size_t MIN_DATA_SIZE = 4;
        const size_t FILE_HEADER_SIZE = 14;
        const size_t OFFSET_TO_OFFSET_TO_DATA = 10;
        const size_t OFFSET_TO_DATA_SIZE = 4;
        const size_t IMAGE_HEADER_SIZE_SIZE = 4;

        if (data.size() < MIN_HEADERS_SIZE + MIN_DATA_SIZE) {
            throw runtime_error("Bytes array is too small to represent bmp"); 
        }
        if (data[0] != 'B' || data[1] != 'M') {
            throw runtime_error("Signature is invalid. First two bytes must be BM");
        }

        clear();
        fileHeader.insert(fileHeader.end(), data.begin(), data.begin() + FILE_HEADER_SIZE);

        offsetToData = uintFromBytes(data.begin() + OFFSET_TO_OFFSET_TO_DATA, data.begin() + OFFSET_TO_OFFSET_TO_DATA + OFFSET_TO_DATA_SIZE);
        
        uint imageHeaderSize = uintFromBytes(data.begin() + FILE_HEADER_SIZE, data.begin() + FILE_HEADER_SIZE + IMAGE_HEADER_SIZE_SIZE);
        imageHeader.insert(imageHeader.end(), data.begin() + FILE_HEADER_SIZE, data.begin() + FILE_HEADER_SIZE + imageHeaderSize);
        pallete.insert(pallete.end(), data.begin() + FILE_HEADER_SIZE + imageHeaderSize, data.begin() + offsetToData);

        parseImageHeader();

        imageData.insert(imageData.end(), data.begin() + offsetToData, data.end());        
    }

    BMPImage::BMPImage(const bytes& data) {
        init(data);
    }

    BMPImage::BMPImage(const string& filename) {
        init(readBytesFromFile(filename)); 
    }
}

