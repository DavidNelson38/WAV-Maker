#ifndef MUSIC_WAVEFORM_H
#define MUSIC_WAVEFORM_H

#include <vector>
using std::vector;

#include "Note.h"
using Music::Note;

namespace Music
{
    class Waveform
    {
    public:
        vector<short> collectSamples(vector<Note> notes, int sampleRate);
    private:
        vector<short> samples;
    };
}

#endif
