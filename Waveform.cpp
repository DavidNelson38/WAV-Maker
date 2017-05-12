// Waveform.cpp
//
// Collects waveform data for each note and chord.  
//

#include "Waveform.h"
using Music::Waveform;

// gathers data for audio file
vector<int16_t>
Music::Waveform::collectSamples(vector<Note> &notes, const int sampleRate)
{      
    bool isItAChordNote;    // is the current note part of a chord? 
    
    for (int loopCounter = 0; (size_t)loopCounter < notes.size();)
    {
        isItAChordNote = notes[loopCounter].getIsItAChordNote();
        // determines the length for each single note
        if (!isItAChordNote)
        {
            int tieIndex;      // index of the next note which may or may not be tied
            double noteLength;

            // skips evaluation of the current note if it is tied to a chord note that is behind it
            while ((size_t)loopCounter < notes.size() && notes[loopCounter].getNoteEndVal() > 0)
            {
                ++loopCounter;
            }
            if ((size_t)loopCounter < notes.size())
                noteLength = notes[loopCounter].getNoteLength();
            tieIndex = loopCounter;
            ++tieIndex;
            if ((size_t)tieIndex < notes.size())
            {
                bool addLength; // should the note's length be added to another note?

                addLength = notes[tieIndex].getAddLength();
                isItAChordNote = notes[tieIndex].getIsItAChordNote();
                // adds the length of each note in the tie sequence
                while (addLength && !isItAChordNote && (size_t)tieIndex < notes.size())
                {
                    noteLength += notes[tieIndex].getNoteLength();
                    notes[loopCounter].setNoteEndVal(noteLength);
                    // sets the note's end frame in case it is tied to a chord in front of it. The chord will need the 
                    // note's end frame to set its start frame
                    notes[tieIndex].setNoteEndVal(noteLength);
                    notes[tieIndex].setSkipNote(true);
                    ++tieIndex;
                    if ((size_t)tieIndex < notes.size())
                    {
                        addLength = notes[tieIndex].getAddLength();
                        isItAChordNote = notes[tieIndex].getIsItAChordNote();
                    }
                }
            }
            loopCounter = tieIndex;
        }
        // determines the length for each chord note
        else
        {
            evaluateTies(notes, loopCounter);
            ++loopCounter;
        }
    }

    for (int loopCounter = 0; (size_t)loopCounter < notes.size(); ++loopCounter)
    {
        const int BIT_RANGE_UPPER_LIMIT = 32767;
        const double PI = 3.14159;

        double noteStartVal = notes[loopCounter].getNoteStartVal(); // the frame that the note starts on
        double noteEndVal = notes[loopCounter].getNoteEndVal();     // the frame that the note ends on
        double amplitude;
        double noteFrequency;
        bool skipNote = notes[loopCounter].getSkipNote();           // has the data for the current note already been 
                                                                    // collected (i.e., was it tied to a previous note)?

        isItAChordNote = notes[loopCounter].getIsItAChordNote();
        // sets the current note's end frame
        if (!isItAChordNote)
        {
            if (noteEndVal == 0)
                noteEndVal = notes[loopCounter].getNoteLength();
        }
        amplitude = notes[loopCounter].getAmplitude();
        noteFrequency = notes[loopCounter].getNoteFrequency();
        // collect waveform data for the current single note
        if (!skipNote && !isItAChordNote)
        {
            for (double loopCntr = noteStartVal; (int)loopCntr < (int)noteEndVal; ++loopCntr)
                samples.push_back((int16_t)(BIT_RANGE_UPPER_LIMIT * amplitude * sin(2 * PI * noteFrequency * loopCntr 
                    / sampleRate)));
        }
        // collect waveform data for the current chord
        else if (isItAChordNote)
        {
            notes = evaluateChord(notes, loopCounter);
            getChordSamples(notes[loopCounter], sampleRate);
        }
    }
  
    return (samples);
}

// determines the length of tied notes
vector<Note>
Music::Waveform::evaluateTies(vector<Note> &notes, const int noteIndex)
{
    int prevNoteIndex = noteIndex - 1;
    double largestChordLength = 0;     // length of the largest chord note (previous chord)    

    // determines the largest chord note. Ignores notes that are not tied, but does not consider the tie when evaluating 
    // the length of a note
    if (prevNoteIndex >= 0 && notes[prevNoteIndex].getIsItAChordNote() == true)
    {
        for (int loopCounter = 0; (size_t)loopCounter < notes[noteIndex].chord.size(); ++loopCounter)
        {
            for (int loopCntr = 0; (size_t)loopCntr < notes[prevNoteIndex].chord.size(); ++loopCntr)
            {
                if ((int)notes[prevNoteIndex].chord[loopCntr].getNoteFrequency() ==
                    (int)notes[noteIndex].chord[loopCounter].getNoteFrequency() 
                    && notes[prevNoteIndex].chord[loopCntr].getAddLength() == true)
                {
                    // prevents skipped chord notes from having a length greater than zero
                    if (notes[prevNoteIndex].chord[loopCntr].getNoteEndVal() == 0 
                        && notes[prevNoteIndex].chord[loopCntr].getSkipNote() == false)
                        notes[prevNoteIndex].chord[loopCntr].setNoteEndVal(
                            notes[prevNoteIndex].chord[loopCntr].getNoteStartVal() 
                            + notes[prevNoteIndex].chord[loopCntr].getNoteLength());
                    if ((int)(notes[prevNoteIndex].chord[loopCntr].getNoteEndVal() 
                        - notes[prevNoteIndex].chord[loopCntr].getNoteStartVal()) > (int)largestChordLength)
                        largestChordLength = notes[prevNoteIndex].chord[loopCntr].getNoteLength();
                }
            }
        }
    }
        
    // determines if the current chord is tied to a note/chord that is behind it
    if (prevNoteIndex >= 0)
    {
        for (int loopCounter = 0; (size_t)loopCounter < notes[noteIndex].chord.size(); ++loopCounter)
        {
            for (int loopCntr = 0; (size_t)loopCntr < notes[prevNoteIndex].chord.size(); ++loopCntr)
            {
                if ((int)notes[prevNoteIndex].chord[loopCntr].getNoteFrequency()
                    == (int)notes[noteIndex].chord[loopCounter].getNoteFrequency()
                    && (notes[prevNoteIndex].chord[loopCntr].getTieToChord() == true 
                        || (notes[prevNoteIndex].chord[loopCntr].getIsItAChordNote() == true 
                            && notes[prevNoteIndex].chord[loopCntr].getAddLength() == true)))
                {
                    // is the chord note tied to a single note? 
                    // sets the chord note's start and end frames
                    if (notes[prevNoteIndex].getIsItAChordNote() == false 
                        && notes[prevNoteIndex].getSkipEnharmonic() == false)
                    {
                        if (notes[prevNoteIndex].getNoteEndVal() == 0)
                            notes[prevNoteIndex].setNoteEndVal(notes[prevNoteIndex].getNoteLength());
                        notes[noteIndex].chord[loopCounter].setNoteStartVal(notes[prevNoteIndex].getNoteEndVal());
                        notes[noteIndex].chord[loopCounter].setNoteEndVal(
                            notes[noteIndex].chord[loopCounter].getNoteStartVal() 
                            + notes[noteIndex].chord[loopCounter].getNoteLength());
                        // prevents the single note from being tied to more than one enharmonic equivalent
                        notes[prevNoteIndex].setSkipEnharmonic(true);
                    }
                    // chord note is tied to another chord note
                    // sets the chord note's start and end frames
                    else if (notes[prevNoteIndex].chord[loopCntr].getSkipEnharmonic() == false)
                    {
                        // notes on a chord cannot begin playing at the same time if two or more of the notes are tied and
                        // the tie sequences are of varying lengths
                        //
                        // the length of the current chord note is added to the length of the previous chord note (the 
                        // starting note in the tie sequence). After this happens, the next chord note in the sequence has 
                        // its length added and so on. This ends when the starting note's length is equal to the length of 
                        // the largest chord note. Since their lengths have already been added, the notes are ignored on 
                        // their respective chords. The entire process that was just described does not happen if the 
                        // current chord note is tied to the largest note
                        if ((int)notes[prevNoteIndex].chord[loopCntr].getNoteLength() < (int)largestChordLength 
                            || (int)(notes[prevNoteIndex].chord[loopCntr].getNoteEndVal() 
                                - notes[prevNoteIndex].chord[loopCntr].getNoteStartVal()) < (int)largestChordLength)
                        {
                            // the length of the starting note
                            double tieLength1 = notes[prevNoteIndex].chord[loopCntr].getNoteEndVal() 
                                - notes[prevNoteIndex].chord[loopCntr].getNoteStartVal();
                            // the difference in length between the starting note and the largest chord note
                            double tieLength2 = largestChordLength - tieLength1;
                            bool isAddLengthTrue = notes[noteIndex].chord[loopCounter].getAddLength();
                            bool endLoop = false;

                            for (int lpCntr = noteIndex; (size_t)lpCntr < notes.size() && endLoop == false; ++lpCntr)
                            {
                                for (int cntr = 0; (size_t)cntr < notes[lpCntr].chord.size() && endLoop == false; ++cntr)
                                {
                                    if ((isAddLengthTrue == true || lpCntr == noteIndex) 
                                        && notes[lpCntr].getIsItAChordNote() == true 
                                        && (int)notes[prevNoteIndex].chord[loopCntr].getNoteFrequency() 
                                        == (int)notes[lpCntr].chord[cntr].getNoteFrequency())
                                    {
                                        isAddLengthTrue = notes[lpCntr].chord[cntr].getAddLength();

                                        if ((int)tieLength2 <= (int)notes[lpCntr].chord[cntr].getNoteLength() 
                                            && notes[lpCntr].chord[cntr].getSkipNote() == false)
                                        {
                                            notes[prevNoteIndex].chord[loopCntr].setNoteEndVal(
                                                notes[prevNoteIndex].chord[loopCntr].getNoteStartVal() 
                                                + largestChordLength);
                                            notes[noteIndex].chord[loopCounter].setNoteStartVal(
                                                notes[prevNoteIndex].chord[loopCntr].getNoteEndVal());
                                            notes[noteIndex].chord[loopCounter].setNoteEndVal(
                                                notes[noteIndex].chord[loopCounter].getNoteStartVal() 
                                                + notes[lpCntr].chord[cntr].getNoteLength() - tieLength2);
                                            // note is ignored
                                            notes[lpCntr].chord[cntr].setSkipNote(true);
                                            endLoop = true;
                                        }
                                        else if (notes[lpCntr].chord[cntr].getSkipNote() == false)
                                        {
                                            tieLength1 += notes[lpCntr].chord[cntr].getNoteLength();
                                            // note is ignored
                                            notes[lpCntr].chord[cntr].setSkipNote(true);
                                            tieLength2 = largestChordLength - tieLength1;
                                            notes[prevNoteIndex].chord[loopCntr].setNoteEndVal(
                                                notes[prevNoteIndex].chord[loopCntr].getNoteStartVal() + tieLength1);
                                        }
                                    }
                                }
                            }
                            // prevents the previous chord note from being tied to more than one enharmonic equivalent
                            notes[prevNoteIndex].chord[loopCntr].setSkipEnharmonic(true);
                        }
                        // the previous chord note in the tie sequence has its start frame set to the default value
                        else if (notes[prevNoteIndex].chord[loopCntr].getNoteStartVal() == 0 
                            && notes[prevNoteIndex].chord[loopCntr].getSkipEnharmonic() == false)
                        {
                            notes[noteIndex].chord[loopCounter].setNoteStartVal(
                                notes[prevNoteIndex].chord[loopCntr].getNoteLength());
                            notes[noteIndex].chord[loopCounter].setNoteEndVal(
                                notes[noteIndex].chord[loopCounter].getNoteStartVal() 
                                + notes[noteIndex].chord[loopCounter].getNoteLength());
                            // prevents the previous chord note from being tied to more than one enharmonic equivalent
                            notes[prevNoteIndex].chord[loopCntr].setSkipEnharmonic(true);
                        }
                        // the previous chord note in the tie sequence had its start frame changed from the default value
                        else if (notes[prevNoteIndex].chord[loopCntr].getSkipEnharmonic() == false)
                        {
                            notes[noteIndex].chord[loopCounter].setNoteStartVal(
                                notes[prevNoteIndex].chord[loopCntr].getNoteEndVal());
                            notes[noteIndex].chord[loopCounter].setNoteEndVal(
                                notes[noteIndex].chord[loopCounter].getNoteStartVal()
                                + notes[noteIndex].chord[loopCounter].getNoteLength());
                            // prevents the previous chord note from being tied to more than one enharmonic equivalent
                            notes[prevNoteIndex].chord[loopCntr].setSkipEnharmonic(true);
                        }
                    }
                }
            }
        }
    }

    // determines if the current chord is tied to a single note that is in front of it
    for (int loopCounter = 0; (size_t)loopCounter < notes[noteIndex].chord.size(); ++loopCounter)
    {
        int nextNoteIndex = noteIndex + 1;

        if (notes[noteIndex].chord[loopCounter].getAddLength() == true && (size_t)nextNoteIndex < notes.size()
            && notes[nextNoteIndex].getIsItAChordNote() == false 
            && (int)notes[noteIndex].chord[loopCounter].getNoteFrequency() == (int)notes[nextNoteIndex].getNoteFrequency())
        {
                notes[noteIndex].chord[loopCounter].setNoteEndVal(notes[noteIndex].chord[loopCounter].getNoteStartVal() 
                    + notes[noteIndex].chord[loopCounter].getNoteLength() + notes[nextNoteIndex].getNoteLength());
                // changing the single note's end frame will skip that note's evaluation
                notes[nextNoteIndex].setNoteEndVal(notes[noteIndex].chord[loopCounter].getNoteEndVal());
                notes[nextNoteIndex].setSkipNote(true);

                int tempNextNoteIndex = ++nextNoteIndex;

                while ((size_t)tempNextNoteIndex < notes.size() && notes[tempNextNoteIndex].getIsItAChordNote()
                    == false && notes[tempNextNoteIndex].getAddLength() == true)
                {
                    notes[noteIndex].chord[loopCounter].setNoteEndVal(notes[noteIndex].chord[loopCounter].getNoteEndVal() 
                        + notes[tempNextNoteIndex].getNoteLength());
                    // changing the single note's end frame will skip that note's evaluation
                    notes[tempNextNoteIndex].setNoteEndVal(notes[noteIndex].chord[loopCounter].getNoteEndVal());
                    notes[tempNextNoteIndex].setSkipNote(true);
                    ++tempNextNoteIndex;
                }
        }
    }

    return(notes);
}

// determines which notes are played alongside each chord note
vector<Note>
Music::Waveform::evaluateChord(vector<Note> &notes, const int noteIndex)
{
    double noteLength1, noteLength2;

    for (int loopCounter = 0; (size_t)loopCounter < notes[noteIndex].chord.size(); ++loopCounter)
    {
        noteLength1 = notes[noteIndex].chord[loopCounter].getNoteEndVal();
        if (noteLength1 == 0 && notes[noteIndex].chord[loopCounter].getSkipNote() == false)
            noteLength1 = notes[noteIndex].chord[loopCounter].getNoteLength();
        noteLength1 -= notes[noteIndex].chord[loopCounter].getNoteStartVal();
        for (int loopCntr = 0; (size_t)loopCntr < notes[noteIndex].chord.size(); ++loopCntr)
        {
            noteLength2 = notes[noteIndex].chord[loopCntr].getNoteEndVal();
            if (noteLength2 == 0 && notes[noteIndex].chord[loopCntr].getSkipNote() == false)
                noteLength2 = notes[noteIndex].chord[loopCntr].getNoteLength();
            noteLength2 -= notes[noteIndex].chord[loopCntr].getNoteStartVal();
            // the note index is stored in the current chord note if the note is played alongside it   
            if ((int)noteLength1 == (int)noteLength2 || (int)noteLength1 < (int)noteLength2)
                notes[noteIndex].chord[loopCounter].chordIndices.push_back(loopCntr);
        }
    }

    return(notes);
}

// obtains chord data
void
Music::Waveform::getChordSamples(Note &note, const int sampleRate)
{
    vector<size_t> indices; // each element represents the number of notes played alongside an unspecified chord note

    for (int loopCounter = 0; (size_t)loopCounter < note.chord.size(); ++loopCounter)
    {
        indices.push_back(note.chord[loopCounter].chordIndices.size());
    }

    size_t endLoop = (size_t)(note.chord.size() - 1);

    // a sorting loop. Elements are arranged from highest to lowest
    for (int loopCounter = 0; (size_t)loopCounter < endLoop;)
    {
        for (int loopCntr = 0; (size_t)loopCntr < endLoop; ++loopCntr)
        {
            if (indices[loopCntr] < indices[loopCntr + 1])
            {
                size_t temp = indices[loopCntr + 1];
                indices[loopCntr + 1] = indices[loopCntr];
                indices[loopCntr] = temp;
            }
        }
        --endLoop;
    }

    // each element represents the number of notes played alongside an unspecified chord note. The loop specifies the chord 
    // note and assigns its index to the element. After every chord note has had its index assigned, the elements will be 
    // in the order of smallest chord note to largest chord note  
    for (int loopCounter = 0, loopCntr = 0; (size_t)loopCounter < indices.size() && (size_t)loopCntr < note.chord.size(); 
        ++loopCntr)
    {
        if (indices[loopCounter] == note.chord[loopCntr].chordIndices.size())
        {
            indices[loopCounter] = loopCntr;
            ++loopCounter;
            loopCntr = -1;
        }
    }
    
    double noteEndVal = note.chord[indices[0]].getNoteEndVal();
    double noteStartVal = note.chord[indices[0]].getNoteStartVal();
    bool skip = false;                                              // a chord note is skipped if its length is equal 
                                                                    // to the previous note's length (all of its 
                                                                    // contributions to the waveform have already been 
                                                                    // processed)  

    if (noteEndVal == 0 && note.chord[indices[0]].getSkipNote() == false)
        noteEndVal = note.chord[indices[0]].getNoteLength();
    // collect waveform data for the current chord
    for (int loopCounter = 0; (size_t)loopCounter < indices.size(); ++loopCounter)
    {
        int frameIncrement = 0; // ensures that the correct frame is used to take the sample point

        if (!skip)
        {
            for (double noteStart = noteStartVal, noteEnd = noteEndVal; (int)noteStart < (int)noteEnd; ++noteStart)
            {
                int16_t samplePoint = 0;

                ++frameIncrement;
                // every note that is playing contributes to the sample point that is taken at each frame 
                for (int loopCntr = 0; (size_t)loopCntr < note.chord[indices[loopCounter]].chordIndices.size(); ++loopCntr)
                {
                    const int BIT_RANGE_UPPER_LIMIT = 32767;
                    const double PI = 3.14159;

                    double frame;
                    double amplitude;
                    double noteFrequency;

                    frame = note.chord[note.chord[indices[loopCounter]].chordIndices[loopCntr]].getNoteStartVal();
                    amplitude = note.chord[note.chord[indices[loopCounter]].chordIndices[loopCntr]].getAmplitude();
                    noteFrequency = note.chord[note.chord[indices[loopCounter]].chordIndices[loopCntr]].getNoteFrequency();

                    samplePoint += (int16_t)(BIT_RANGE_UPPER_LIMIT * amplitude * sin(2 * PI * noteFrequency * (frame 
                        + frameIncrement) / sampleRate));
                }
                samples.push_back(samplePoint);
            }
        }
        // skips the next chord note if its length is equal to the current note's length (all of its contributions to the 
        // waveform have already been processed)
        if ((size_t)(loopCounter + 1) < indices.size())
        {
            double tempNoteEndVal = note.chord[indices[loopCounter + 1]].getNoteEndVal();
            double tempNoteStartVal = note.chord[indices[loopCounter + 1]].getNoteStartVal();
            
            if (tempNoteEndVal == 0 && note.chord[indices[loopCounter + 1]].getSkipNote() == false)
                tempNoteEndVal = note.chord[indices[loopCounter + 1]].getNoteLength();
            if ((int)(tempNoteEndVal - tempNoteStartVal) > (int)(noteEndVal - noteStartVal))
            {
                noteStartVal = noteEndVal;
                noteEndVal = noteStartVal + tempNoteEndVal - tempNoteStartVal;
                skip = false;
            }
            else
                skip = true;
        }
    }
}
