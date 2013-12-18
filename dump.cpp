#include <iostream>

#include "bmpimage.h"

using namespace std;

int main(int argc, char** argv) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " filename" << endl;
        return 1;
    }
    
    cout << NImage::dumpToHex(NImage::readBytesFromFile(argv[1]));

    return 0;
}

