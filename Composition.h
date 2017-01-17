#ifndef MUSIC_COMPOSITION_H
#define MUSIC_COMPOSITION_H

#include <vector>
using std::vector;

#include "Measure.h"
using Music::Measure;

namespace Music
{
    class Composition
    {
    public:
        Composition();
        void addMeasure(int beatsPerMeasure, int timeSigDenominator, map<string, int> *scientificPtr, 
            map<int, double> *frequencyPtr);
        int getBeatsPerMeasure() const;
        vector<Measure> measures;
    private:
        int beatsPerMeasure;
    };
}

#endif
