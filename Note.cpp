#include "Note.h";
using Music::Note;

const int NO_PITCH = 0;

bool Music::Note::isItTied = false;
double Music::Note::samePitch = NO_PITCH;

Music::Note::Note(double amplitude, double noteFrequency, double noteLength, bool isItTied) : 
    amplitude(amplitude), noteFrequency(noteFrequency), noteLength(noteLength)
{
    if (this->isItTied && samePitch == this->noteFrequency)
    {
        addLength = true;
        this->isItTied = false;
    }
    else
        addLength = false;
    if (isItTied)
        this->isItTied = true;
    samePitch = this->noteFrequency;
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
Music::Note::getAddLength() const
{
    return (addLength);
}
