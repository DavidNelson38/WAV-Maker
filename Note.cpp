// Note.cpp
//
// A single note or chord.  
//

#include "Note.h"
using Music::Note;

bool Music::Note::isItTied = false;
double Music::Note::samePitch = 0;

Music::Note::Note(double amplitude, double noteFrequency, double noteLength, double noteEndVal, double noteStartVal, 
    bool skipNote, bool skipEnharmonic, bool isItAChordNote, bool addLength, bool tieToChord, bool isItTied) : 
    amplitude(amplitude), noteFrequency(noteFrequency), noteLength(noteLength), noteEndVal(noteEndVal), 
    noteStartVal(noteStartVal), skipNote(skipNote), skipEnharmonic(skipEnharmonic), isItAChordNote(isItAChordNote), 
    addLength(addLength), tieToChord(tieToChord)
{
    if (!isItAChordNote)
    {
        // the note length will be added to the previous note length if the current single note is tied and has the same 
        // pitch as the previous single note
        if (this->isItTied && (int)samePitch == (int)this->noteFrequency)
        {
            this->addLength = true;
            this->isItTied = false;
        }
        // ensures that if the next note is a single note, it will have its pitch compared to the current note's pitch 
        if (isItTied)
        {
            this->tieToChord = true;
            this->isItTied = true;
        }
        samePitch = this->noteFrequency;
    }
    else
    {
        if (isItTied)
            this->addLength = true;
    }
}

void
Music::Note::setNoteEndVal(const double noteLength)
{
    noteEndVal = noteLength;
}

void
Music::Note::setNoteStartVal(const double noteLength)
{
    noteStartVal = noteLength;
}

void
Music::Note::setAmplitude(const double amplitude)
{
    this->amplitude = amplitude;
}

void
Music::Note::setSkipNote(const bool skipNote)
{
    this->skipNote = skipNote;
}

void
Music::Note::setSkipEnharmonic(const bool skipEnharmonic)
{
    this->skipEnharmonic = skipEnharmonic;
}

double
Music::Note::getNoteEndVal() const
{
    return (noteEndVal);
}

double
Music::Note::getNoteStartVal() const
{
    return (noteStartVal);
}

double
Music::Note::getAmplitude() const
{
    return (amplitude);
}

double
Music::Note::getNoteFrequency() const
{
    return (noteFrequency);
}

double
Music::Note::getNoteLength() const
{
    return (noteLength);
}

bool
Music::Note::getTieToChord() const
{
    return (tieToChord);
}

bool
Music::Note::getAddLength() const
{
    return (addLength);
}

bool
Music::Note::getIsItAChordNote() const
{
    return (isItAChordNote);
}

bool
Music::Note::getSkipNote() const
{
    return (skipNote);
}

bool
Music::Note::getSkipEnharmonic() const
{
    return (skipEnharmonic);
}
