// CLOCK - Language and Terminal for Microcontrollers
// Copyright (C) 2024 Agustin Do Canto

#include <iostream>
#include <fstream>
#include <string>
#include <cctype>
#include "clockInterpreter.h"

#define DELETE_CODE 127
#define ENTER_CODE 13

using namespace std;

ClockInterpreter interpreter;

std::string dataReceivedFromKeyboard = "";
bool data_received = false;
bool running_code = false;

void run_cli(char c) {
    data_received = true;

    if (!running_code) {
        switch (c) {
            case DELETE_CODE:
                if (!dataReceivedFromKeyboard.empty()) {
                    dataReceivedFromKeyboard.pop_back();
                }
                break;

            case '\n':
            case ENTER_CODE:
                if (dataReceivedFromKeyboard.substr(0, 3) == "RUN") {
                    string filename = dataReceivedFromKeyboard.substr(4);
                    running_code = true;
                
                    interpreter.run(filename);

                    dataReceivedFromKeyboard.clear();
                    running_code = false;
                    cout << "\n> ";
                } 

                break;

            default:
                if (c != '\t' && c != '\r') {
                    dataReceivedFromKeyboard += c;
                }
        }
    }
}

int main() {
    cout << "CLOCK - Language and Terminal for Microcontrollers" << "\nCopyright (C) 2024 Agustin Do Canto\n" << endl;
    cout << "> ";

    while (true) {
        char c = cin.get();
        run_cli(c);
    }

    return 0;
}
