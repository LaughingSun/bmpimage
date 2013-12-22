#include <string>
#include <iostream>
#include <stdexcept>

#include "lsb_encoder.h"

using NImage::LSBEncoder;
using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::cerr;
using std::exception;

int main() {
    string filename;
    string message;
    string outputFilename;

    cout << "Input filename" << endl;
    cin >> filename; 

    LSBEncoder encoder(filename);
    size_t canEncode = encoder.getMaximumLength();
    if (canEncode == 0) {
        cout << "Sorry can't encode in this file" << endl;
        return 0;
    }

    cout << "Can encode " << canEncode << " bits. Input message (only ones and zeros are allowed)" << endl;
    cin >> message;

    if (message.size() > canEncode) {
        cout << "Inputted message has length " << message.size() << ", but maximum length is only " << canEncode << endl;
    }

    cout << "Input filename for encoded file" << endl;
    cin >> outputFilename;

    try {
        encoder.encodeBitMessage(message);
        encoder.writeMessageToFile(outputFilename);
    } catch (const exception& e) {
        cerr << "Something went wrong:" << endl;
        cerr << "Error message: " << e.what() << endl;
    }

    return 0;
}

