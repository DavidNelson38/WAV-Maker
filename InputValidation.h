// InputValidation.h
//
// Functions verify that the user's input is valid. 
//

#ifndef MUSIC_INPUT_VALIDATION_H
#define MUSIC_INPUT_VALIDATION_H

#include <string>
using std::string;

#include <vector>
using std::vector;

namespace Music
{
    // confirms that user entered a number
    void verifyUserInput(const string userInput);
    // confirms that user set the beat equal to a valid note
    void verifyTimeSigDenominator(int userInput);
    // confirms that the chord note entered in by the user does not have the same pitch as the other notes in the chord
    void verifyChordPitch(const bool isItAChordNote, double noteFrequency);
    // clears chord frequencies stored in this namespace
    void resetChordFrequencies();
    // clears the chord length stored in this namespace
    void resetChordLength();
    // if necessary, sets the chord length to the note length entered in by the user
    double verifyChordLength(double measureLength, double maxMeasureLength, double noteLength);
    int convertCharToInt(string userInput);
    string convert2DArrayToString(const int row, const string inputArray[][4]);

    static vector<double> chordFrequencies; // frequencies of current chord
    static double chordLength = 0;          // length of current chord
    static double saveChordLength = 0;      // saves the current chord length before it's altered
}

#endif
