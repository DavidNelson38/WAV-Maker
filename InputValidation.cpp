// InputValidation.cpp
//
// Functions verify that the user's input is valid. 
//

#include <stdexcept>
using std::runtime_error;

#include "InputValidation.h"
using Music::verifyUserInput;
using Music::verifyTimeSigDenominator;
using Music::verifyChordPitch;
using Music::resetChordFrequencies;
using Music::resetChordLength;
using Music::verifyChordLength;
using Music::convertCharToInt;
using Music::convert2DArrayToString;

// confirms that user entered a number
void
Music::verifyUserInput(const string userInput)
{
    for (int loopCounter = 0; userInput[loopCounter] != '\0'; ++loopCounter)
    {
        if (!isdigit(userInput[loopCounter]))
            throw runtime_error("Invalid input.");
    }
}

// confirms that user set the beat equal to a valid note
void
Music::verifyTimeSigDenominator(int userInput)
{
    const double BASE_TWO = 2; // base two number system

    double beatExponent;       // 2^exp = time signature denominator

    beatExponent = log10((double)userInput) / log10(BASE_TWO);
    if (beatExponent - (int)beatExponent != 0)
        throw runtime_error("Number must be a power of two.");
}

// confirms that the chord note entered in by the user does not have the same pitch as the other notes in the chord
void
Music::verifyChordPitch(const bool isItAChordNote, double noteFrequency)
{
    if (isItAChordNote)
    {
        for (int loopCounter = 0; (size_t)loopCounter < chordFrequencies.size(); ++loopCounter)
        {
            if ((int)noteFrequency == (int)chordFrequencies[loopCounter])
                throw runtime_error("Chord notes cannot have the same letter unless they are in different octaves.");
        }
        chordFrequencies.push_back(noteFrequency);
    }
}

// clears chord frequencies stored in this namespace
void
Music::resetChordFrequencies()
{
    chordFrequencies.clear();
}

// clears the chord length stored in this namespace
void
Music::resetChordLength()
{
    chordLength = 0;
    saveChordLength = 0;
}

// if necessary, sets the chord length to the note length entered in by the user
double
Music::verifyChordLength(double measureLength, double maxMeasureLength, double noteLength)
{
    // chord length increases, which changes the measure length
    if ((int)noteLength > (int)chordLength)
    {
        chordLength = noteLength;
        // previous chord length is removed from the measure length
        measureLength -= saveChordLength;
        if ((int)measureLength > (int)maxMeasureLength)
        {
            measureLength -= noteLength + saveChordLength;
            chordLength = saveChordLength;
            throw runtime_error("Exceeds beats per measure.");
        }
        saveChordLength = chordLength;
    }
    // chord length remains the same. Measure length does not change
    else
    {
        // note length removed since measure length does not change
        measureLength -= noteLength;
    }
   
    return (measureLength);
}

int
Music::convertCharToInt(string userInput)
{
    const int ASCII_TO_INTEGER = 48;  // converts the ASCII character code to the integer that is represented by char 
    const int BASE_TEN_DEFAULT = 1;   // default value for base ten number system
    const int BASE_TEN = 10;          // multiplier for base ten number system 

    int convertedUserInput = 0;
    int baseTen = BASE_TEN_DEFAULT;

    for (int loopCounter = userInput.length() - 1; loopCounter >= 0; --loopCounter)
    {
        convertedUserInput += ((int)userInput[loopCounter] - ASCII_TO_INTEGER) * baseTen;
        baseTen *= BASE_TEN;
    }

    return (convertedUserInput);
}

string 
Music::convert2DArrayToString(const int row, const string inputArray[][4])
{
    string userInput;

    for (int loopCounter = 0; inputArray[row][loopCounter] != "\0"; ++loopCounter)
        userInput += inputArray[row][loopCounter];

    return (userInput);
}
