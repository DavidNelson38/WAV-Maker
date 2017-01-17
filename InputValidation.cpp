#include <cmath>
using std::log10;

#include <stdexcept>
using std::runtime_error;

#include "InputValidation.h"
using Music::verifyUserInput;
using Music::verifyTimeSigDenominator;
using Music::convertCharToInt;
using Music::convert2DArrayToString;

#include <iostream>
using std::cout;

void
Music::verifyUserInput(string userInput)
{
    for (int loopCounter = 0; userInput[loopCounter] != '\0'; ++loopCounter)
    {
        if (!isdigit(userInput[loopCounter]))
            throw runtime_error("Invalid input.");
    }
}

void
Music::verifyTimeSigDenominator(int userInput)
{
    const int NOT_DIVISIBLE = 0;
    const int WHOLE_NOTE = 1;
    const double BASE_TWO = 2;

    double beatExponent;

    beatExponent = log10((double)userInput) / log10(BASE_TWO);
    if (beatExponent - (int)beatExponent != NOT_DIVISIBLE)
        throw runtime_error("Number must be a power of two.");
}

int
Music::convertCharToInt(string userInput)
{
    const int ASCII_TO_INTEGER = 48;
    const int BASE_TEN_DEFAULT = 1;
    const int BASE_TEN = 10;
    const int USER_INPUT_DEFAULT = 0;

    int convertedUserInput = USER_INPUT_DEFAULT;
    int baseTen = BASE_TEN_DEFAULT;

    for (int loopCounter = userInput.length() - 1; loopCounter >= 0; --loopCounter)
    {
        convertedUserInput += ((int)userInput[loopCounter] - ASCII_TO_INTEGER) * baseTen;
        baseTen *= BASE_TEN;
    }

    return (convertedUserInput);
}

string 
Music::convert2DArrayToString(int row, string inputArray[][4])
{
    string userInput;

    for (int loopCounter = 0; inputArray[row][loopCounter] != "\0"; ++loopCounter)
        userInput += inputArray[row][loopCounter];

    return (userInput);
}
