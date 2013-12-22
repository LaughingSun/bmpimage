#pragma once

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <algorithm>


using std::endl;
using std::min;
using std::ifstream;
using std::ios;
using std::ostream;
using std::ofstream;
using std::runtime_error;
using std::string;
using std::stringstream;
using std::vector;

namespace NImage {
    typedef unsigned char byte;
    typedef vector<byte> bytes;
    typedef unsigned int uint;

    bytes readBytesFromFile(const string& filename) {  
        const size_t BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE]; 
        ifstream in(filename.c_str(), ios::in | ios::binary);

        bytes result;
        in.read(buffer, BUFFER_SIZE);
        while (in) {
            result.insert(result.end(), buffer, buffer + BUFFER_SIZE);
            in.read(buffer, BUFFER_SIZE);
        }
        result.insert(result.end(), buffer, buffer + in.gcount());

        return result;
    }

    void writeBytesToFile(const string& filename, const bytes& data) {
        const size_t BUFFER_SIZE = 1024;
        char buffer[BUFFER_SIZE];
        ofstream out(filename.c_str(), ios::out | ios::binary);
        
        size_t written = 0;
        while (written < data.size()) {
            size_t lim = min(BUFFER_SIZE, data.size() - written);
            for (size_t i = 0; i < lim; ++i) {
                buffer[i] = data[written++];
            }

            out.write(buffer, lim);
        }
        out.close();
    }

    char digitToHex(byte digit) {
        if (digit >= 16) {
            throw runtime_error("Digit must be in range [0..15]");
        }
        if (digit < 10) return digit + '0';
        return digit - 10 + 'a';
    }

    string toHex(byte value) {
        return string() + digitToHex(value >> 4) + digitToHex(value & 15);
    }

    string dumpToHex(const bytes& data) {
        stringstream ss;
        for (size_t i = 0; i < data.size(); ++i) {
            ss << toHex(data[i]);
            if (i + 1 == data.size()) break;

            if ((i & 15) == 15) {
                ss << '\n';
            } else if ((i & 7) == 7) {
                ss << " | ";
            } else if ((i & 1) == 1) {
                ss << ' ';
            }
        }
        ss << endl;
        return ss.str();
    }

    uint uintFromBytes(const bytes& data) {
        if (data.size() > 4) {
            throw runtime_error("uint can't store more than 4 bytes");
        }
        uint result = 0;
        for (int i = int(data.size()) - 1; i >= 0; --i) {
            result = (result << 8) + data[i]; 
        }
        return result;
    }

    void bytesFromUint(bytes::iterator begin, bytes::iterator end, uint value) {
        while (begin < end) {
            *begin = value & 255;
            value >>= 8;
            ++begin;
        }
    }

    uint uintFromBytes(bytes::const_iterator begin, bytes::const_iterator end) {
        return uintFromBytes(bytes(begin, end));
    }
}
