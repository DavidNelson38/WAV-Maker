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
        int tempo;
        int sampleRate;
        int beatsPerMeasure;
        int timeSigDenominator;
        double beatLength;
    };
}

#endif
