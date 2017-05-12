// Composition.cpp
//
// A musical composition. The song's measures are stored in this class. 
//

#include <iostream>
using std::cerr;
using std::cin;
using std::cout;

#include <stdexcept>
using std::runtime_error;

#include "Composition.h"
using Music::Composition;

#include "InputValidation.h"
using Music::resetChordLength;

Music::Composition::Composition()
{
    const int ASCII_TO_INTEGER = 48; // converts the ASCII character code to the integer that is represented by char  
    const int BASE_TEN_DEFAULT = 1;  // default value for base ten number system
    const int BASE_TEN = 10;         // multiplier for base ten number system

    beatsPerMeasure = 0;

    // asks user for beats per measure and interprets input
    while (beatsPerMeasure == 0)
    {
        int baseTen = BASE_TEN_DEFAULT;

        try
        {
            string userInput;

            cout << "How many beats per measure? ";
            cin >> userInput;
            for (int loopCounter = 0; userInput[loopCounter] != '\0'; ++loopCounter)
            {
                if (!isdigit(userInput[loopCounter]))
                    throw runtime_error("Number of beats must be entered.");
            }
            // converts beats per measure string to an integer
            // formula: 10^n * x1 + 10^(n + 1) * x2 ..., x is a string char
            for (int loopCounter = userInput.length() - 1; loopCounter >= 0; --loopCounter)
            {
                beatsPerMeasure += ((int)userInput[loopCounter] - ASCII_TO_INTEGER) * baseTen;
                baseTen *= BASE_TEN;
            }
            if (beatsPerMeasure == 0)
                throw runtime_error("Number of beats must be greater than zero.");
        }
        catch (runtime_error &exception)
        {
            cerr << exception.what() << "\n\n";
        }
    }
}

void
Music::Composition::addMeasure(int beatsPerMeasure, int timeSigDenominator, map<string, int> *scientificPtr, 
    map<int, double> *frequencyPtr)
{
    Measure measure(beatsPerMeasure, timeSigDenominator, 0, scientificPtr, frequencyPtr);

    measures.push_back(measure); 
    resetChordLength();          // resets the chord length since a new measure has been started 
}

int
Music::Composition::getBeatsPerMeasure() const
{
    return (beatsPerMeasure);
}
