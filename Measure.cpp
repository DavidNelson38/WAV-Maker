// Measure.cpp
//
// A song measure. 
//

#include <iostream>
using std::cerr;
using std::cin;
using std::cout;

#include <stdexcept>
using std::runtime_error;

#include "Display.h"
using Music::addDisplayString;
using Music::addSpace;

#include "InputValidation.h"
using Music::verifyUserInput;
using Music::convertCharToInt;
using Music::convert2DArrayToString;

#include "Measure.h"
using Music::Measure;

Music::Measure::Measure(int beatsPerMeasure, int timeSigDenominator, double currentMeasureLength, 
    map<string, int> *scientificPtr, map<int, double> *frequencyPtr) :
    TimeSignature(beatsPerMeasure, timeSigDenominator), currentMeasureLength(currentMeasureLength), 
    scientificPtr(scientificPtr), frequencyPtr(frequencyPtr)
{
    getMaxMeasureLength();
}

void
Music::Measure::getMaxMeasureLength()
{
    int beatsPerMeasure = getBeatsPerMeasure();
    double beatLength = getBeatLength();

    maxMeasureLength = beatLength * beatsPerMeasure;
}

Note
Music::Measure::addNote(const string userInput, const bool isItAChordNote, const bool skipNote, const bool isItANewNote, 
    MeasureDisplay &measureDisplay)
{
    const int AFTER_PITCH_INDEX = 2;

    string pitch;

    // stores pitch information into a string
    for (int loopCounter = 0; loopCounter < AFTER_PITCH_INDEX && (size_t)loopCounter < userInput.size(); ++loopCounter)
    {
        pitch += userInput[loopCounter];
    }
    try
    {
        string noteLength;
        string sharpOrFlat;
        string amplitude;
        string dotted;
        string endOfArray;
        string noteAttributes[][4] =
        {
            { sharpOrFlat },
            { amplitude },
            { dotted },
            { endOfArray }
        };
        int frequencyIndex;
        int loopCntr;
        int noteAttributesIndex = 0;
        int attributeIndex = 0;
        double noteFrequency;
        bool isASharp = false;
        bool isItDotted = false;
        bool isItTied = false;

        // searches map for key. The element associated with the key will be used to find the correct frequency
        if ((*scientificPtr).find(pitch) == (*scientificPtr).end())
            throw runtime_error("Not a valid pitch.");
        frequencyIndex = (*scientificPtr)[pitch];
        noteFrequency = (*frequencyPtr)[frequencyIndex];

        // clears chord frequencies if the user is on a new single note or new chord 
        if (isItANewNote)
            resetChordFrequencies();
        // confirms that the chord note entered in by the user does not have the same pitch as the other notes in the chord
        verifyChordPitch(isItAChordNote, noteFrequency);

        // stores note length information into a string
        for (loopCntr = AFTER_PITCH_INDEX; (size_t)loopCntr < userInput.size() && isdigit(userInput[loopCntr]); 
            ++loopCntr)
            noteLength += userInput[loopCntr];

        // clears chord length if the user is on a new single note or new chord
        if (isItANewNote)
            resetChordLength();

        // searches user input for information on the current note and stores the data in the appropriate strings
        for (int loopCounter = loopCntr; (size_t)loopCounter < userInput.size(); ++loopCounter)
        {
            switch (userInput[loopCounter])
            {
                case 's':
                    noteAttributesIndex = 0;
                    attributeIndex = 0;
                    isASharp = true;
                    break;
                case 'b':
                    noteAttributesIndex = 0;
                    attributeIndex = 0;
                    isASharp = false;
                    break;
                case 'a':
                    noteAttributesIndex = 1;
                    attributeIndex = 0;
                    break;
                case 'd':
                    noteAttributesIndex = 2;
                    attributeIndex = 0;
                    isItDotted = true;
                    break;
                case 't':
                    isItTied = true;
                    break;
                default:
                    noteAttributes[noteAttributesIndex][attributeIndex] += userInput[loopCounter];
                    ++attributeIndex;
                    break;
            }
        }
        
        const int MAX_AMPLITUDE = 1;
        const int VOLUME_TO_AMPLITUDE = 100;
        const int DOT_DIVISOR = 2;

        int noteDenominator;
        int frequencyIndexModifier;
        int saveFrequencyIndex;
        int dots;
        double timeSigDenominator;
        double beatFactor;
        double noteLen;
        double measureLength;
        double saveCurrentMeasureLength;
        double amp;
        double addedNoteLength;
        
        // confirms that user entered a number for the note length
        verifyUserInput(noteLength);
        noteDenominator = convertCharToInt(noteLength);
        // if no note length was entered by the user, throw exception
        if (!noteDenominator)
            throw runtime_error("Invalid input.");
        timeSigDenominator = getTimeSigDenominator();
        saveCurrentMeasureLength = currentMeasureLength;
        // time signature note is smaller than current note (does not consider tied notes or dots)
        if ((int)timeSigDenominator / noteDenominator > 1)
        {
            beatFactor = timeSigDenominator / noteDenominator;
            // if time signature note does not divide evenly by current note, throw exception
            if (beatFactor - (int)beatFactor != 0)
                throw runtime_error("Note length must be divisible by the beat.");
            noteLen = getBeatLength() * beatFactor;
            measureLength = currentMeasureLength + noteLen;
            if ((int)measureLength > (int)maxMeasureLength && !isItAChordNote)
                throw runtime_error("Exceeds beats per measure.");
            // if necessary, sets the chord length to the note length entered in by the user
            if (isItAChordNote)
                currentMeasureLength = verifyChordLength(measureLength, maxMeasureLength, noteLen);
            else
                currentMeasureLength = measureLength;
        }
        // time signature note is longer than current note (does not consider tied notes or dots)
        else if ((int)timeSigDenominator / noteDenominator < 1)
        {
            beatFactor = noteDenominator / timeSigDenominator;
            // if current note does not divide evenly by time signature note, throw exception
            if (beatFactor - (int)beatFactor != 0)
                throw runtime_error("Note length must be a multiple of the beat.");
            noteLen = getBeatLength() / beatFactor;
            measureLength = currentMeasureLength + noteLen;
            if ((int)measureLength > (int)maxMeasureLength && !isItAChordNote)
                throw runtime_error("Exceeds beats per measure.");
            // if necessary, sets the chord length to the note length entered in by the user
            if (isItAChordNote)
                currentMeasureLength = verifyChordLength(measureLength, maxMeasureLength, noteLen);
            else
                currentMeasureLength = measureLength;
        }
        // time signature note is the same length as the current note (does not consider tied notes or dots)
        else
        {
            noteLen = getBeatLength();
            measureLength = currentMeasureLength + noteLen;
            if ((int)measureLength > (int)maxMeasureLength && !isItAChordNote)
                throw runtime_error("Exceeds beats per measure.");
            // if necessary, sets the chord length to the note length entered in by the user
            if (isItAChordNote)
                currentMeasureLength = verifyChordLength(measureLength, maxMeasureLength, noteLen);
            else
                currentMeasureLength = measureLength;
        }

        sharpOrFlat = convert2DArrayToString(0, noteAttributes);
        // confirms that user entered a number for sharps/flats
        verifyUserInput(sharpOrFlat);
        frequencyIndexModifier = convertCharToInt(sharpOrFlat);
        saveFrequencyIndex = frequencyIndex;
        // changes key if note has one or more sharps. The element associated with the key will be used to find the correct 
        // frequency 
        if (isASharp)
            frequencyIndex += frequencyIndexModifier;
        // changes key if note has one or more flats. The element associated with the key will be used to find the correct 
        // frequency
        else
        {
            frequencyIndex -= frequencyIndexModifier;
        }
        if (frequencyIndex < 0 || (size_t)frequencyIndex > (*frequencyPtr).size() - 1)
            throw runtime_error("Invalid input.");
        else
            noteFrequency = (*frequencyPtr)[frequencyIndex];

        amplitude = convert2DArrayToString(1, noteAttributes);
        // confirms that user entered a number for volume
        verifyUserInput(amplitude);
        // sets volume to the maximum value if user did not enter a number 
        if (amplitude.size() == 0)
            amp = MAX_AMPLITUDE;
        else
        {
            amp = (double)convertCharToInt(amplitude);
            // converts volume to amplitude
            amp /= VOLUME_TO_AMPLITUDE;
            // sets amplitude to the maximum value to prevent integer overflow
            if (amp > MAX_AMPLITUDE)
                amp = MAX_AMPLITUDE;
        }

        if (isItDotted)
        {
            dotted = convert2DArrayToString(2, noteAttributes);
            verifyUserInput(dotted);
            // confirms that user entered a number for dots
            dots = convertCharToInt(dotted);
            addedNoteLength = noteLen;
            // dots increase note length
            for (int loopCounter = 0; loopCounter < dots; ++loopCounter)
            {
                addedNoteLength /= DOT_DIVISOR;
                noteLen += addedNoteLength;
            }
            measureLength = saveCurrentMeasureLength + noteLen;
            if ((int)measureLength > (int)maxMeasureLength && !isItAChordNote)
                throw runtime_error("Exceeds beats per measure.");
            // if necessary, sets the chord length to the note length entered in by the user
            if (isItAChordNote)
                currentMeasureLength = verifyChordLength(measureLength, maxMeasureLength, noteLen);
            else
                currentMeasureLength = measureLength;
        }

        // There are one hundred nineteen possible frequencies, but only seventy can be represented as natural notes. The 
        // following code assigns the note frequency to a natural note (C-D-E-F-G-A-B) in the appropriate octave.
        // Formula: Maximum Note Frequency_nat. notes - (Maximum Note Frequency_all notes - Current Note Frequency) / 2

        const int MAX_FREQUENCY_INDEX = 119;
        const int FREQUENCY_INTERVAL = 12;
        const int STARTING_FREQUENCY_INDEX = 69;

        int frequencyDiff = MAX_FREQUENCY_INDEX - saveFrequencyIndex;
        int intervalEndpoint = MAX_FREQUENCY_INDEX - FREQUENCY_INTERVAL;
        int staffIndex;
        int staffIndexModifier1 = 0;
        double staffIndexModifier2;

        // corrects the frequency assignment formula. For every twelve frequencies, the correction increases by one
        while (intervalEndpoint >= saveFrequencyIndex)
        {
            intervalEndpoint -= FREQUENCY_INTERVAL;
            ++staffIndexModifier1;
        }
        staffIndexModifier2 = (double)frequencyDiff / 2;
        // number is rounded to the nearest one before being converted to an int
        staffIndexModifier2 += staffIndexModifier2 - (frequencyDiff / 2);
        staffIndex = STARTING_FREQUENCY_INDEX - (int)staffIndexModifier2 - staffIndexModifier1;

        Note note(amp, noteFrequency, noteLen, 0, 0, false, false, isItAChordNote, false, isItTied, isItTied);

        // adds space between the current single note/chord and the previous single note/chord
        addSpace(measureDisplay, staffIndex, noteFrequency, note.getAddLength(), isItAChordNote, isItANewNote, 
            note.getTieToChord());
        // adds the note type to the display
        addDisplayString(measureDisplay, staffIndex, noteLength, 0, 0, isItAChordNote, isASharp);
        // adds the note's sharps/flats to the display
        if (frequencyIndexModifier > 0)
            addDisplayString(measureDisplay, staffIndex, noteLength, 1, frequencyIndexModifier, isItAChordNote, isASharp);
        // adds the note's dots to the display
        if (isItDotted)
            addDisplayString(measureDisplay, staffIndex, noteLength, 2, dots, isItAChordNote, isASharp);

        // note is added to the current measure
        if (!skipNote)
        {
            notes.push_back(note);
            return (note);
        }
        // note is added to the current chord
        else
            return (note);
    }
    catch (runtime_error &exception)
    {
        Note note(0, 0, 0, 0, 0, false, false, false, false, false, false);

        cerr << exception.what() << "\n";
        
        return (note);
    }
}

// indicates whether the measure is finished or not
bool
Music::Measure::moveToNextMeasure()
{
    bool makeANewMeasure = false;

    if ((int)currentMeasureLength == (int)maxMeasureLength)
    {
        makeANewMeasure = true;
    }

    return (makeANewMeasure);
}
