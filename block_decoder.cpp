#include <iostream>
#include <string>

#include "block_decoder.h"

using std::cout;
using std::cerr;
using std::endl;
using std::string;
using NImage::BlockDecoder;

int main(int argc, char** argv) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " filename" << endl;
        return 0;
    }

    BlockDecoder decoder(argv[1]);
    string result = decoder.decodeBitMessage();
    if (result.size() == 0) {
        cout << "Can't find message" << endl;
        return 0;
    }

    cout << "Encoded bits: " << result << endl;

    return 0;
}

