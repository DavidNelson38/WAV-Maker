// Note.h
//
// A single note or chord. 
//

#ifndef MUSIC_NOTE_H
#define MUSIC_NOTE_H

#include <vector>
using std::vector;

namespace Music
{
    class Note
    {
    public:
        Note(double amplitude, double noteFrequency, double noteLength, double noteEndVal, double noteStartVal, 
            bool skipNote, bool skipEnharmonic, bool isItAChordNote, bool addLength, bool tieToChord, bool isItTied);
        void setNoteEndVal(const double noteLength);
        void setNoteStartVal(const double noteLength);
        void setAmplitude(const double amplitude);
        void setSkipNote(const bool skipNote);
        void setSkipEnharmonic(const bool skipEnharmonic);
        double getNoteEndVal() const;
        double getNoteStartVal() const;
        double getAmplitude() const;
        double getNoteFrequency() const;
        double getNoteLength() const;
        bool getTieToChord() const;
        bool getAddLength() const;
        bool getIsItAChordNote() const;
        bool getSkipNote() const;
        bool getSkipEnharmonic() const;
        vector<Note> chord;             // stores all of the chord notes
        vector<int> chordIndices;       // each chord note remembers what other chord notes are played with it
    private:
        double amplitude;
        double noteFrequency;
        double noteLength;
        double noteEndVal;       // the frame that the note ends on
        double noteStartVal;     // the frame that the note starts on
        bool skipNote;           // should the note be added to the current measure or to the current chord?
        bool skipEnharmonic;     // is the enharmonic equivalent already tied?
        bool isItAChordNote;     // is the current note part of a chord?
        bool addLength;          // should the note's length be added to another note?
        bool tieToChord;         // is the current note tied to a chord?
        static bool isItTied;    // is the current note tied to another note?
        static double samePitch; // does the current note/chord have the same pitch as the previous note/chord?
    };
}

#endif
