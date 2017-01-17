#include <windows.h>
#include <mmsystem.h>

#include <fstream>
using std::ios_base;
using std::fstream;

#include <iostream>
using std::cerr;
using std::cout;
using std::cin;

#include <stdexcept>
using std::runtime_error;

#include <string>
using std::string;

#include "Composition.h"
using Music::Composition;

#include "Frequencies.h"
using Music::Frequency;

#include "InputValidation.h"
using Music::verifyUserInput;
using Music::convertCharToInt;
using Music::verifyTimeSigDenominator;

#include "TimeSignature.h"
using Music::TimeSignature;

#include "WAV.h"
using Music::WAV;

#include "Waveform.h"
using Music::Waveform;

int main()
{
    string userInput;
    int timeSigDenominator;
    bool isInvalidInput = true;

    while (isInvalidInput)
    {
        try
        {
            cout << "Which note represents a beat? ";
            cin >> userInput;
            verifyUserInput(userInput);
            timeSigDenominator = convertCharToInt(userInput);
            verifyTimeSigDenominator(timeSigDenominator);
            isInvalidInput = false;
        }
        catch (runtime_error &exception)
        {
            cerr << exception.what() << "\n\n";
            isInvalidInput = true;
        }
    }

    Composition musicSheets;
    Frequency soundFrequencies;
    int beatsPerMeasure = musicSheets.getBeatsPerMeasure();
    map<string, int> *scientificPtr = soundFrequencies.getScientific();
    map<int, double> *frequenciesPtr = soundFrequencies.getFrequencies();

    musicSheets.addMeasure(beatsPerMeasure, timeSigDenominator, scientificPtr, frequenciesPtr);
    for (int loopCounter = 0; loopCounter < 4; ++loopCounter)
        musicSheets.measures[0].addNote();

    int sampleRate = musicSheets.measures[0].getSampleRate();
    Waveform soundData;
    vector<short> samples = soundData.collectSamples(musicSheets.measures[0].notes, sampleRate);
   
    fstream MusicFile("music.wav", ios_base::binary | ios_base::out | ios_base::trunc);
    if (!MusicFile.is_open())
    {
        cerr << "WAV file failed to open." << "\n\n";
        exit(EXIT_FAILURE);
    }
    WAV musicFileData((unsigned int)sampleRate, samples, MusicFile);

    MusicFile.close();
    //PlaySound(TEXT("music.wav"), NULL, SND_FILENAME);
   
    return(EXIT_SUCCESS);
}
