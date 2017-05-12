// Display.cpp
//
// Outputs the song's measures to the console.
//

#include "Display.h"
using Music::MeasureDisplay;

// adds note to measure
void
Music::addDisplayString(MeasureDisplay &measureDisplay, const int staffIndex, const string displayString, 
    const int displayCode, const int userInput, const bool isItAChordNote, const bool isASharp)
{
    const int NOTE_LENGTH = 0;
    const int SHARP_OR_FLAT = 1;
    const int DOTTED = 2;

    // adds the note type to the display
    if (displayCode == NOTE_LENGTH)
        measureDisplay.staff[staffIndex] += displayString;
    // adds the note's sharps/flats to the display
    else if (displayCode == SHARP_OR_FLAT)
    {
        for (int loopCounter = 0; loopCounter < userInput; ++loopCounter)
        {
            if (isASharp)
                measureDisplay.staff[staffIndex] += "s";
            else
                measureDisplay.staff[staffIndex] += "b";
        }
    }
    // adds the note's dots to the display
    else if (displayCode == DOTTED)
    {
        for (int loopCounter = 0; loopCounter < userInput; ++loopCounter)
        {
            measureDisplay.staff[staffIndex] += ".";
        }
    }

    measureDisplay.prevStaffIndex = staffIndex;
    measureDisplay.noteStartCharIndex = measureDisplay.staff[staffIndex].rfind(" ");
    measureDisplay.noteEndCharIndex = measureDisplay.staff[staffIndex].find_last_not_of(" ");
    measureDisplay.prevIsItAChord = isItAChordNote;

    if (measureDisplay.staff[staffIndex].size() > measureDisplay.largestLineLength)
        measureDisplay.largestLineLength = measureDisplay.staff[staffIndex].size();
    if (staffIndex < measureDisplay.minStaffIndex)
        measureDisplay.minStaffIndex = staffIndex;
    if (staffIndex > measureDisplay.maxStaffIndex)
        measureDisplay.maxStaffIndex = staffIndex;
}

// spaces notes
void
Music::addSpace(MeasureDisplay &measureDisplay, const int staffIndex, double noteFrequency, const bool addLength, 
    const bool isItAChordNote, const bool isItANewNote, const bool tieToChord)
{
    static bool alignTies;

    // ensures the current note will be properly aligned. Does not consider ties
    if (measureDisplay.prevStaffIndex > 0)
    {
        size_t staffDiff; // difference in length between the largest/previous line/space of the measure and the current 
                          // line/space of the measure

        // chord note will start above or below previous note
        if (isItAChordNote && !isItANewNote)
        {
            staffDiff = (size_t)measureDisplay.noteStartCharIndex - (measureDisplay.staff[staffIndex].size() - 1);
            measureDisplay.staff[staffIndex].append(staffDiff, ' ');
        }
        // the note's start position is one ahead of the previous note's start position     
        else
        {
            staffDiff = measureDisplay.largestLineLength - measureDisplay.staff[staffIndex].size();
            measureDisplay.staff[staffIndex].append(staffDiff, ' ');
        }
    }
    // ensures that tied notes on a chord will be properly aligned 
    if (isItANewNote)
       alignTies = false;

    // finalizes the spacing for single notes
    if (!isItAChordNote || isItANewNote)
    {
        // adds the default spacing between notes
        measureDisplay.staff[staffIndex] += measureDisplay.space;
        // determines if note is tied to previous note and adds additional spacing. Alignment specifications depend on 
        // whether the previous note is part of a chord or not and whether the notes belong to different measures or not
        if (!isItAChordNote && (addLength || measureDisplay.prevIsItAChord == true))
        {
            if (measureDisplay.prevIsItAChord == false)
            {
                measureDisplay.staff[staffIndex].insert(measureDisplay.noteEndCharIndex + 1, "  tied");
                measureDisplay.staff[staffIndex].erase(measureDisplay.staff[staffIndex].size() - 2, 2);
            }
            else
            {
                if (measureDisplay.savePrevFrequencies.size() == 0)
                    measureDisplay.savePrevFrequencies = measureDisplay.prevFrequencies;
                for (int loopCounter = 0; (size_t)loopCounter < measureDisplay.savePrevFrequencies.size(); ++loopCounter)
                {
                    if ((int)noteFrequency == (int)measureDisplay.savePrevFrequencies[loopCounter])
                    {
                        // single note is tied to a chord note that is in the same measure
                        if (measureDisplay.largestLineLength > 0)
                        {
                            measureDisplay.staff[staffIndex].insert(measureDisplay.largestLineLength + 1, " tied");
                            measureDisplay.staff[staffIndex].erase(measureDisplay.staff[staffIndex].size() - 1, 1);
                        }
                        // single note is tied to a chord note that is in a different measure 
                        else
                        {
                            measureDisplay.staff[staffIndex].insert(measureDisplay.largestLineLength + 1, "  tied");
                            measureDisplay.staff[staffIndex].erase(measureDisplay.staff[staffIndex].size() - 2, 2);
                        }
                    }
                }
            }
        }
    }

    // before starting a new chord, the note positions of the previous chord must be removed 
    if (isItAChordNote && isItANewNote)
        measureDisplay.chordIndices.clear();
    // stores position of current chord note
    if (isItAChordNote)
        measureDisplay.chordIndices.push_back(staffIndex);
    if (isItANewNote && (measureDisplay.prevFrequencies.size() > 0 || measureDisplay.savePrevFrequencies.size() > 0))
        ++measureDisplay.resetPrevFrequencies;
    // the frequency(ies) of the previous note/chord are saved so that the program can begin storing the frequency(ies) of
    // the current note/chord
    if (measureDisplay.resetPrevFrequencies == 2)
    {
        measureDisplay.prevFrequencies = measureDisplay.savePrevFrequencies;
        measureDisplay.savePrevFrequencies.clear();
        measureDisplay.resetPrevFrequencies = 1;
    }
    // stores note frequency, but only if it is tied to another note
    if (measureDisplay.resetPrevFrequencies == 1)
    {
        if (isItAChordNote && addLength)
            measureDisplay.savePrevFrequencies.push_back(noteFrequency);
        else if (!isItAChordNote && tieToChord)
            measureDisplay.savePrevFrequencies.push_back(noteFrequency);
    }
    // stores note frequency, but only if it is tied to another note. The frequency will be compared to the frequency(ies) 
    // of the next single note/chord  
    if (measureDisplay.resetPrevFrequencies == 0)
    {
        if (isItAChordNote && addLength)
            measureDisplay.prevFrequencies.push_back(noteFrequency);
        else if (!isItAChordNote && tieToChord)
            measureDisplay.prevFrequencies.push_back(noteFrequency);
    }

    // finalizes the spacing for chords
    if (isItAChordNote && measureDisplay.resetPrevFrequencies != 0)
    {
        size_t lineSize = measureDisplay.staff[staffIndex].size();

        for (int loopCounter = 0; (size_t)loopCounter < measureDisplay.prevFrequencies.size(); ++loopCounter)
        {
            // determines if chord note is tied to previous chord and adds additional spacing
            if ((int)noteFrequency == (int)measureDisplay.prevFrequencies[loopCounter])
            {
                // no need to align ties because this is the first note of the chord to be tied
                if (alignTies == false)
                {
                    measureDisplay.staff[staffIndex].insert(measureDisplay.staff[staffIndex].size() - 2, "tied");
                    alignTies = true;
                }
                // aligns the chord ties
                else
                {
                    measureDisplay.staff[staffIndex].insert(measureDisplay.staff[staffIndex].size() - 6, "tied");
                    measureDisplay.staff[staffIndex].erase(measureDisplay.staff[staffIndex].size() - 6, 4);
                }
                // realigns chord notes after notes have been tied
                for (int loopCntr = 0; loopCntr < (int)measureDisplay.chordIndices.size() - 1; ++loopCntr)
                {
                    measureDisplay.staff[measureDisplay.chordIndices[loopCntr]].insert(lineSize - 1, measureDisplay.space);
                }
                // removes notes that have been realigned so that they are not further realigned if any of the next chord 
                // notes are tied
                measureDisplay.chordIndices.clear();
                // removes the previous note in the tie sequence to prevent it from being tied to more than one enharmonic 
                // equivalent
                measureDisplay.prevFrequencies.erase(measureDisplay.prevFrequencies.begin() + loopCounter);
            }
        }
    }
}

// displays measures
ostream &
Music::operator<<(ostream &out, const MeasureDisplay &measureDisplay)
{
    out << "  Measure " << measureDisplay.measureNum << "    Time Signature " << measureDisplay.beatsPerMeasure << "/" 
        << measureDisplay.timeSigDenominator << "\n\n";

    // displays the current measure. Lines and spaces that do not fall within the song's range of pitches are not 
    // displayed
    for (int loopCounter = measureDisplay.maxStaffIndex; loopCounter >= measureDisplay.minStaffIndex; --loopCounter)
    {
        if (measureDisplay.staff[loopCounter].size() > 1)
            out << measureDisplay.staff[loopCounter] << "\n";
        else
            out << " \n";
    }

    if (measureDisplay.isShowCommand == false)
    {
        if (measureDisplay.isItAChord)
            out << "\n  Chord: On\n";
        else
            out << "\n  Chord: Off\n";
    }

    return (out);
}

Music::MeasureDisplay::MeasureDisplay(int prevStaffIndex, int noteStartCharIndex, int noteEndCharIndex, 
    int resetPrevFrequencies, int measureNum, int beatsPerMeasure, int timeSigDenominator, int minStaffIndex, 
    int maxStaffIndex, size_t largestLineLength, bool prevIsItAChord, bool isShowCommand) : 
    prevStaffIndex(prevStaffIndex), noteEndCharIndex(noteEndCharIndex), resetPrevFrequencies(resetPrevFrequencies), 
    measureNum(measureNum), beatsPerMeasure(beatsPerMeasure), timeSigDenominator(timeSigDenominator), 
    minStaffIndex(minStaffIndex), maxStaffIndex(maxStaffIndex), largestLineLength(largestLineLength), 
    prevIsItAChord(prevIsItAChord), isShowCommand(isShowCommand)
{
    const int STAFF_VECTOR_SIZE = 70;
    const int END_OF_OCTAVE = 0;
    const int LAST_LETTER = 6;

    int noteLetter = END_OF_OCTAVE;

    // creates a staff for each octave included in the frequency range of human hearing (approx. 20 Hz to 20 kHz). c10, the 
    // final note in this range, is excluded 
    for (int loopCounter = 0; loopCounter < STAFF_VECTOR_SIZE; ++loopCounter)
    {
        switch (noteLetter)
        {
            case (0):
                staff.push_back("C");
                ++noteLetter;
                break;
            case (1):
                staff.push_back("D");
                ++noteLetter;
                break;
            case (2):
                staff.push_back("E");
                ++noteLetter;
                break;
            case (3):
                staff.push_back("F");
                ++noteLetter;
                break;
            case (4):
                staff.push_back("G");
                ++noteLetter;
                break;
            case (5):
                staff.push_back("A");
                ++noteLetter;
                break;
            case (6):
                staff.push_back("B");
                ++noteLetter;
                break;
        }
        if (noteLetter > LAST_LETTER)
            noteLetter = END_OF_OCTAVE;
    }
    // default spacing between notes
    space = "    ";
}

// copies required variables into new measure
void 
Music::MeasureDisplay::operator=(const MeasureDisplay &measureDisplay)
{
    // needed for a single note that is tied to a chord from another measure
    prevIsItAChord = measureDisplay.prevIsItAChord;
    // needed for a chord note that is tied to a chord from another measure
    resetPrevFrequencies = measureDisplay.resetPrevFrequencies;
    // needed for a chord note that is tied to a chord from another measure
    savePrevFrequencies = measureDisplay.savePrevFrequencies;
    // needed for a chord note that is tied to a chord from another measure
    prevFrequencies = measureDisplay.prevFrequencies;
}

void
Music::MeasureDisplay::setIsItAChord(bool isItAChord) 
{
    this->isItAChord = isItAChord;
}

void
Music::MeasureDisplay::setisShowCommand(bool isShowCommand)
{
    this->isShowCommand = isShowCommand;
}
