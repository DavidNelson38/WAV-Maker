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

Music::TimeSignature::TimeSignature(int beatsPerMeasure, int timeSigDenominator) : 
    beatsPerMeasure(beatsPerMeasure), timeSigDenominator(timeSigDenominator)
{
    const int NO_TEMPO = 0;
    const int ASCII_TO_INTEGER = 48;
    const int BASE_TEN_DEFAULT = 1;
    const int BASE_TEN = 10;
    const int SAMPLE_RATE = 44100;
    const int SECONDS_TO_MINUTES = 60;

    string userInput;
    tempo = NO_TEMPO;
    int baseTen = BASE_TEN_DEFAULT;
    sampleRate = SAMPLE_RATE;

    while (tempo == 0)
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

    beatLength = (double)sampleRate * SECONDS_TO_MINUTES / tempo;
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
