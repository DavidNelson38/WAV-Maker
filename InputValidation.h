#ifndef MUSIC_INPUT_VALIDATION_H
#define MUSIC_INPUT_VALIDATION_H

#include <string>
using std::string;

namespace Music
{
    void verifyUserInput(string userInput);
    void verifyTimeSigDenominator(int userInput);
    int convertCharToInt(string userInput);
    string convert2DArrayToString(int row, string inputArray[][4]);
}

#endif
