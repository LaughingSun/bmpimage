#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdexcept>

using std::vector;
using std::string;
using std::ifstream;
using std::endl;
using std::runtime_error;
using std::ios;
using std::stringstream;


namespace NImage {
    typedef unsigned char byte;
    typedef vector<byte> bytes;

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
}
