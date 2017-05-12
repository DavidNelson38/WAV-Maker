// TimeSignature.cpp
//
// Time signature for a musical composition. 
//

#include <iostream>
using std::cerr;
using std::cin;
using std::cout;

#include <stdexcept>
using std::runtime_error;

#include <string>
using std::string;

#include "TimeSignature.h"
using Music::TimeSignature;

double Music::TimeSignature::beatLength;
bool Music::TimeSignature::isThereATempo = false;

Music::TimeSignature::TimeSignature(int beatsPerMeasure, int timeSigDenominator) : 
    beatsPerMeasure(beatsPerMeasure), timeSigDenominator(timeSigDenominator)
{
    const int ASCII_TO_INTEGER = 48;      // converts the ASCII character code to the integer that is represented by char
    const int BASE_TEN_DEFAULT = 1;       // default value for base ten number system
    const int BASE_TEN = 10;              // multiplier for base ten number system
    const int SAMPLE_RATE = 44100;        // frames per second
    const double SECONDS_TO_MINUTES = 60;

    string userInput;
    tempo = 0;
    int baseTen = BASE_TEN_DEFAULT;
    sampleRate = SAMPLE_RATE;

    // asks user for tempo and interprets input
    while (tempo == 0 && !isThereATempo)
    {
        try
        {
            cout << "What is the tempo? ";
            cin >> userInput;
            for (int loopCounter = 0; userInput[loopCounter] != '\0'; ++loopCounter)
            {
                if (!isdigit(userInput[loopCounter]))
                    throw runtime_error("Tempo must be a number.");
            }
            // converts tempo string to an integer
            // formula: 10^n * x1 + 10^(n + 1) * x2 ..., x is a string char
            for (int loopCounter = userInput.length() - 1; loopCounter >= 0; --loopCounter)
            {
                tempo += ((int)userInput[loopCounter] - ASCII_TO_INTEGER) * baseTen;
                baseTen *= BASE_TEN;
            }
            if (tempo == 0)
                throw runtime_error("Tempo must be greater than zero.");
        }
        catch (runtime_error &exception)
        {
            cerr << exception.what() << "\n\n";
            baseTen = BASE_TEN_DEFAULT;
        }
    }
    
    // calculates beat length
    if (isThereATempo == false)
    {
        beatLength = sampleRate * SECONDS_TO_MINUTES / tempo;
        // prevents tempo and beat length from being recalculated every time an object of this class is instantiated
        isThereATempo = true;
    }
}

int
Music::TimeSignature::getBeatsPerMeasure() const
{
    return (beatsPerMeasure);
}

int
Music::TimeSignature::getSampleRate() const
{
    return (sampleRate);
}

int
Music::TimeSignature::getTimeSigDenominator() const
{
    return (timeSigDenominator);
}

double
Music::TimeSignature::getBeatLength() const
{
    return (beatLength);
}
