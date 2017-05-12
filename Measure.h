// Measure.h
//
// A song measure. 
//

#ifndef MUSIC_MEASURE_H
#define MUSIC_MEASURE_H

#include <map>
using std::map;

#include "Display.h"
using Music::MeasureDisplay;

#include "Note.h"
using Music::Note;

#include "TimeSignature.h"
using Music::TimeSignature;

namespace Music
{
    class Measure : public TimeSignature
    {
    public:
        Measure(int beatsPerMeasure, int timeSigDenominator, double currentMeasureLength, map<string, int> *scientificPtr, 
            map<int, double> *frequencyPtr);
        void getMaxMeasureLength();
        Note addNote(const string userInput, const bool isItAChordNote, const bool skipNote, const bool isItANewNote, 
            MeasureDisplay &measureDisplay);
        // indicates whether the measure is finished or not
        bool moveToNextMeasure();
        vector<Note> notes;
    private:
        double currentMeasureLength;     // the current length of the measure
        double maxMeasureLength;         // the maximum, allowable length of the measure
        map<string, int> *scientificPtr; 
        map<int, double> *frequencyPtr;
    };
}

#endif
