// Composition.h
//
// A musical composition. The song's measures are stored in this class. 
//

#ifndef MUSIC_COMPOSITION_H
#define MUSIC_COMPOSITION_H

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
