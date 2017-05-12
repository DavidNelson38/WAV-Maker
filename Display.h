// Display.h
//
// Outputs the song's measures to the console. 
//

#ifndef MUSIC_DISPLAY_H
#define MUSIC_DISPLAY_H

#include <ostream>
using std::ostream;

#include <string>
using std::string;

#include <vector>
using std::vector;

namespace Music
{
    class MeasureDisplay
    {
        // adds note to measure
        friend void addDisplayString(MeasureDisplay &measureDisplay, const int staffIndex, const string displayString, 
            const int userInput, const int displayCode, const bool isItAChordNote, const bool isASharp);
        // spaces notes
        friend void addSpace(MeasureDisplay &measureDisplay, const int staffIndex, double noteFrequency, 
            const bool addLength, const bool isItAChordNote, const bool isItANewNote, const bool tieToChord);
        // displays measures
        friend ostream &operator<<(ostream &out, const MeasureDisplay &measureDisplay);

    public:
        MeasureDisplay(int prevStaffIndex, int noteStartCharIndex, int noteEndCharIndex, int resetPrevFrequencies, 
            int measureNum, int beatsPerMeasure, int timeSigDenominator, int minStaffIndex, int maxStaffIndex, 
            size_t largestLineLength, bool prevIsItAChord, bool isShowCommand);
        // copies required variables into new measure
        void operator=(const MeasureDisplay &measureDisplay);
        void setIsItAChord(bool isItAChord);
        void setisShowCommand(bool isShowCommand);
    private:
        int prevStaffIndex;                 // staff index of the previous note
        int noteStartCharIndex;             // index of the beginning character of a note 
        int noteEndCharIndex;               // index of the final character of a note
        int resetPrevFrequencies;           // signals to clear or store the frequency(ies) of the previous note/chord
        int measureNum;                     // each measure is numbered, starting with one
        int beatsPerMeasure;                // time signature numerator
        int timeSigDenominator;             // time signature denominator
        int minStaffIndex;                  // the first measure line/space to be displayed
        int maxStaffIndex;                  // the last measure line/space to be displayed 
        size_t largestLineLength;           // size of the largest element in the staff vector
        bool isItAChord;                    // T: current note is a chord F: current note is not a chord
        bool prevIsItAChord;                // T: previous note is a chord F: previous note is not a chord
        bool isShowCommand;                 // T: show all measures F: show current measure
        string space;                       // the default space between notes
        vector<int> chordIndices;           // contains the staff indices of the current chord
        vector<double> savePrevFrequencies; // saves the previous frequency(ies) for later use
        vector<double> prevFrequencies;     // contains the previous frequency(ies) of the last note/chord
        vector<string> staff;               // stores the notes of a measure. Each vector element represents the notes on a
                                            // line or space of the measure
    };
}

#endif
