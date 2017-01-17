#include <iostream>
using std::cerr;
using std::cin;
using std::cout;

#include <stdexcept>
using std::runtime_error;

#include <string>
using std::string;

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
Music::Measure::addNote()
{
    bool isInvalidInput = true;
 
    while (isInvalidInput)
    {
        const int AFTER_PITCH_INDEX = 2;

        string userInput;
        string pitch;
        string noteLength;
        string sharpOrFlat;
        string amplitude;
        string dotted;
        string tied;
        string endOfArray;
        string noteAttributes[][4] = 
        {
            {sharpOrFlat}, 
            {amplitude},
            {dotted},
            {endOfArray}
        };
        int loopCntr;
        int frequencyIndex;
        int noteAttributesIndex = 0;
        int attributeIndex = 0;
        double noteFrequency;
        bool isASharp = false;
        bool isItTied = false;
        bool isItDotted = false;

        cout << "Enter a note. ";
        cin >> userInput;
        for (int loopCounter = 0; loopCounter < AFTER_PITCH_INDEX && (size_t)loopCounter < userInput.size(); ++loopCounter)
        {
            pitch += userInput[loopCounter];
        }
        try
        {
            if ((*scientificPtr).find(pitch) == (*scientificPtr).end())
                throw runtime_error("Not a valid pitch.");
            frequencyIndex = (*scientificPtr)[pitch];
            noteFrequency = (*frequencyPtr)[frequencyIndex];

            for (loopCntr = AFTER_PITCH_INDEX; (size_t)loopCntr < userInput.size() && isdigit(userInput[loopCntr]); 
                ++loopCntr)
                noteLength += userInput[loopCntr];

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
           
            const int NO_BEAT_FACTOR = 1;
            const int NOT_DIVISIBLE = 0;
            const int NO_USER_INPUT = 0;
            const int VOLUME_TO_AMPLITUDE = 100;
            const int MAX_AMPLITUDE = 1;
            const int DOT_DIVISOR = 2;

            int noteDenominator;
            int frequencyIndexModifier;
            int dots;
            double addedNoteLength;
            double timeSigDenominator;
            double beatFactor;
            double noteLen;
            double measureLength;
            double amp;

            verifyUserInput(noteLength);
            noteDenominator = convertCharToInt(noteLength);
            if (!noteDenominator)
                throw runtime_error("Invalid input.");
            timeSigDenominator = getTimeSigDenominator();
            if (timeSigDenominator / noteDenominator > NO_BEAT_FACTOR)
            {
                beatFactor = timeSigDenominator / noteDenominator;
                if (beatFactor - (int)beatFactor != NOT_DIVISIBLE)
                    throw runtime_error("Note length must be divisible by the beat.");
                noteLen = getBeatLength() * beatFactor;
                measureLength = currentMeasureLength + noteLen;
                //if (measureLength > maxMeasureLength)
                    //throw runtime_error("Exceeds beats per measure.");
                currentMeasureLength = measureLength;
            }
            else if (timeSigDenominator / noteDenominator < NO_BEAT_FACTOR)
            {
                beatFactor = noteDenominator / timeSigDenominator;
                if (beatFactor - (int)beatFactor != NOT_DIVISIBLE)
                    throw runtime_error("Note length must be a multiple of the beat.");
                noteLen = getBeatLength() / beatFactor;
                measureLength = currentMeasureLength + noteLen;
                //if (measureLength > maxMeasureLength)
                    //throw runtime_error("Exceeds beats per measure.");
                currentMeasureLength = measureLength;
            }
            else
            {
                noteLen = getBeatLength();
                measureLength = currentMeasureLength + noteLen;
                //if (measureLength > maxMeasureLength)
                    //throw runtime_error("Exceeds beats per measure.");
                currentMeasureLength = measureLength;
            }

            sharpOrFlat = convert2DArrayToString(0, noteAttributes);
            verifyUserInput(sharpOrFlat);
            frequencyIndexModifier = convertCharToInt(sharpOrFlat);
            if (isASharp)
                frequencyIndex += frequencyIndexModifier;
            else
            {
                frequencyIndex -= frequencyIndexModifier;
            }
            if (frequencyIndex < 0 || (size_t)frequencyIndex > (*frequencyPtr).size() - 1)
                throw runtime_error("Invalid input.");
            else
                noteFrequency = (*frequencyPtr)[frequencyIndex];

            amplitude = convert2DArrayToString(1, noteAttributes);
            verifyUserInput(amplitude);
            if (amplitude.size() == NO_USER_INPUT)
                amp = MAX_AMPLITUDE;
            else
            {
                amp = (double)convertCharToInt(amplitude);
                amp /= VOLUME_TO_AMPLITUDE;
                if (amp > MAX_AMPLITUDE)
                    amp = MAX_AMPLITUDE;
            }

            if (isItDotted)
            {
                dotted = convert2DArrayToString(2, noteAttributes);
                verifyUserInput(dotted);
                dots = convertCharToInt(dotted);
                addedNoteLength = noteLen;
                for (int loopCounter = 0; loopCounter < dots; ++loopCounter)
                {
                    addedNoteLength /= DOT_DIVISOR;
                    noteLen += addedNoteLength;
                }
                measureLength = currentMeasureLength + noteLen;
                //if (measureLength > maxMeasureLength)
                    //throw runtime_error("Exceeds beats per measure.");
                currentMeasureLength = measureLength;
            }

            Note note(amp, noteFrequency, noteLen, isItTied);
            notes.push_back(note);
            isInvalidInput = false;
        }
        catch (runtime_error &exception)
        {
            cerr << exception.what() << "\n\n";
            isInvalidInput = true;
        }
    }
}

void
Music::Measure::getMaxMeasureLength()
{
    int beatsPerMeasure = getBeatsPerMeasure();
    double beatLength = getBeatLength();

    maxMeasureLength = beatLength * beatsPerMeasure;
}
