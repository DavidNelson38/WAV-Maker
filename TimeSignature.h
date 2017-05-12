// TimeSignature.h
//
// Time signature for a musical composition. 
//

#ifndef MUSIC_TIME_SIGNATURE_H
#define MUSIC_TIME_SIGNATURE_H

namespace Music
{
    class TimeSignature
    {
    public:
        TimeSignature(int beatsPerMeasure, int timeSigDenominator);
        int getBeatsPerMeasure() const;
        int getSampleRate() const;
        int getTimeSigDenominator() const;
        double getBeatLength() const;
    private:
        int tempo;                 // beats per minute
        int sampleRate;            // frames per second
        int beatsPerMeasure;       
        int timeSigDenominator;     
        static double beatLength;   
        static bool isThereATempo; // has the tempo been set? 
    };
}

#endif
