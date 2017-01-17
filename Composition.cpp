#include <iostream>
using std::cerr;
using std::cin;
using std::cout;

#include <stdexcept>
using std::runtime_error;

#include <string>
using std::string;

#include "Composition.h"
using Music::Composition;

Music::Composition::Composition()
{
    const int NO_BEATS = 0;
    const int ASCII_TO_INTEGER = 48;
    const int BASE_TEN_DEFAULT = 1;
    const int BASE_TEN = 10;

    string userInput;
    int baseTen = BASE_TEN_DEFAULT;

    beatsPerMeasure = NO_BEATS;

    while (beatsPerMeasure == 0)
    {
        try
        {
            cout << "How many beats per measure? ";
            cin >> userInput;
            for (int loopCounter = 0; userInput[loopCounter] != '\0'; ++loopCounter)
            {
                if (!isdigit(userInput[loopCounter]))
                    throw runtime_error("Number of beats must be entered.");
            }
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
            baseTen = BASE_TEN_DEFAULT;
        }
    }
}

void
Music::Composition::addMeasure(int beatsPerMeasure, int timeSigDenominator, map<string, int> *scientificPtr, 
    map<int, double> *frequencyPtr)
{
    Measure measure(beatsPerMeasure, timeSigDenominator, 0, scientificPtr, frequencyPtr);

    measures.push_back(measure);
}

int
Music::Composition::getBeatsPerMeasure() const
{
    return (beatsPerMeasure);
}
