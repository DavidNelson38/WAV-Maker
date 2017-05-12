// Waveform.h
//
// Collects waveform data for each note and chord.  
//

#ifndef MUSIC_WAVEFORM_H
#define MUSIC_WAVEFORM_H

#include "Note.h"
using Music::Note;

namespace Music
{
    class Waveform
    {
    public:
        // gathers data for audio file
        vector<int16_t> collectSamples(vector<Note> &notes, const int sampleRate);
        // determines the length of tied notes
        vector<Note> evaluateTies(vector<Note> &notes, const int noteIndex);
        // determines which notes are played alongside each chord note
        vector<Note> evaluateChord(vector<Note> &notes, const int noteIndex);
        // obtains chord data
        void getChordSamples(Note &note, const int sampleRate);
    private:
        vector<int16_t> samples;
    };
}

#endif
