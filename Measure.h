#ifndef MUSIC_MEASURE_H
#define MUSIC_MEASURE_H

#include <map>
using std::map;

#include <vector>
using std::vector;

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
        void addNote();
        void getMaxMeasureLength();
        vector<Note> notes;
    private:
        double currentMeasureLength;
        double maxMeasureLength;
        map<int, double> *frequencyPtr;
        map<string, int> *scientificPtr;
    };
}

#endif
