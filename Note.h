#ifndef MUSIC_NOTE_H
#define MUSIC_NOTE_H

namespace Music
{
    class Note
    {
    public:
        Note(double amplitude, double noteFrequency, double noteLength, bool isItTied);
        double getAmplitude() const;
        double getNoteFrequency() const;
        double getNoteLength() const;
        bool getAddLength() const;
    private:
        double amplitude;
        double noteFrequency;
        double noteLength;
        bool addLength;
        static bool isItTied;
        static double samePitch;
    };
}

#endif
