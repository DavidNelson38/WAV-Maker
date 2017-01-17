#include <cmath>
using std::sin;

#include "Waveform.h"
using Music::Waveform;

vector<short>
Music::Waveform::collectSamples(vector<Note> notes, int sampleRate)
{
    const int BIT_RANGE_UPPER_LIMIT = 32767;
    const double PI = 3.14159;

    int tieIndex;
    double amplitude;
    double noteFrequency;
    double noteLength;
    bool addLength;

    for (int loopCounter = 0; (size_t)loopCounter < notes.size();)
    {
        amplitude = notes[loopCounter].getAmplitude();
        noteFrequency = notes[loopCounter].getNoteFrequency();
        noteLength = notes[loopCounter].getNoteLength();
        tieIndex = loopCounter;
        ++tieIndex;
        if ((size_t) tieIndex < notes.size())
            addLength = notes[tieIndex].getAddLength();
        while (addLength && (size_t)tieIndex < notes.size())
        {
            noteLength += notes[tieIndex].getNoteLength();
            ++tieIndex;
            if ((size_t)tieIndex < notes.size())
                addLength = notes[tieIndex].getAddLength();
        }
        loopCounter = tieIndex;

        for (int loopCounter = 0; loopCounter <= noteLength; ++loopCounter)
            samples.push_back((int)(BIT_RANGE_UPPER_LIMIT * amplitude * sin(2 * PI * noteFrequency * loopCounter 
                / sampleRate)));        
    }

    return (samples);
}
