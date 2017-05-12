// Music.cpp
//
// User can create a musical composition. The data is stored as a WAV file. This program opens a connection to 
// Windows Media Player.
//

#include <experimental/filesystem>
using std::experimental::filesystem::current_path;
using std::experimental::filesystem::path;

#include <fstream>
using std::ios_base;
using std::fstream;

#include <iomanip>
using std::setw;

#include <iostream>
using std::cerr;
using std::cout;
using std::cin;
using std::streamsize;

#include <limits>
using std::numeric_limits;

#include <stdexcept>
using std::runtime_error;

#include "Composition.h"
using Music::Composition;

#include "Frequencies.h"
using Music::Frequency;

#include "InputValidation.h"
using Music::verifyUserInput;
using Music::convertCharToInt;
using Music::verifyTimeSigDenominator;

#include "WAV.h"
using Music::WAV;

#include "Waveform.h"
using Music::Waveform;

// windef.h defines macros min and max. This can cause compiler errors because the standard library has functions with the 
// same common names. Function calls to min or max will be replaced by the corresponding macro during preprocessing. The 
// macro NOMINMAX prevents windef.h from defining these macros and should be included before any Windows headers 
#define NOMINMAX  

#include <atlbase.h> // Windows header file for CComPtr, a smart pointer class that manages COM interface pointers
#include <wmp.h>     // Windows header file for Windows Media Player

int main()
{
    cout << "\n\n" << setw(44) << "WAVMAKER  v2.0\n\n" << setw(46) << "code written\n" << setw(56)
        << "by David Nelson\n\n\n\n" << "If this is your first time using WAVmaker, it is recommended that you view the\n"
        "README before composing.\n\nPress Enter to continue.";
    while (cin.get() != '\n');
    cout << "\n\n\n";

    string userInput;
    int timeSigDenominator;
    bool isInvalidInput = true;

    // sets the beat for the time signature
    while (isInvalidInput)
    {
        try
        {
            cout << "Which note represents a beat? ";
            cin >> userInput;
            // confirms that user entered a number for the beat
            verifyUserInput(userInput);
            timeSigDenominator = convertCharToInt(userInput);
            // confirms that user set the beat equal to a valid note
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
    int measureIndex = 0;
    int displayIndex = 0;
    int measureNum = 1;
    int beatsPerMeasure = musicSheets.getBeatsPerMeasure();
    MeasureDisplay measureDisplay(0, 0, 0, 0, measureNum, beatsPerMeasure, timeSigDenominator, 70, 0, 0, false, false);
    vector<MeasureDisplay> displays;
    map<string, int> *scientificPtr = soundFrequencies.getScientific();
    map<int, double> *frequenciesPtr = soundFrequencies.getFrequencies();
    bool isItAChord = false;

    musicSheets.addMeasure(beatsPerMeasure, timeSigDenominator, scientificPtr, frequenciesPtr);
    displays.push_back(measureDisplay);

    // user composes song
    while (userInput[0] != 'q')
    {
        bool isShowCommand = false; // did the user enter the show command? 

        cout << "Enter a note. ";
        cin >> userInput;
        // user enters a chord note
        if (userInput == "ch")
        {
            isItAChord = true;
            cin >> userInput;
        }
        // displays all measures
        if (userInput == "show")
        {
            isShowCommand = true;
            for (int loopCounter = 0; (size_t)loopCounter < displays.size(); ++loopCounter)
            {
                // ensures that the chord on/off setting is not shown
                displays[loopCounter].setisShowCommand(isShowCommand);
                cout << "\n\n" << displays[loopCounter] << "\n";
                displays[loopCounter].setisShowCommand(false);
            }
        }
        // creates a new measure if the current measure cannot hold anymore notes
        if (isShowCommand == false && musicSheets.measures[measureIndex].moveToNextMeasure() == true)
        {
            MeasureDisplay nextMeasureDisplay(0, 0, 0, 0, ++measureNum, beatsPerMeasure, timeSigDenominator, 70, 0, 0, 0,
                false);

            musicSheets.addMeasure(beatsPerMeasure, timeSigDenominator, scientificPtr, frequenciesPtr);
            ++measureIndex;
            // copies certain variables from the previous measure into the new measure. These variables are needed in case 
            // the first note is tied to the last note of the previous measure
            nextMeasureDisplay = displays[displayIndex];
            ++displayIndex;
            displays.push_back(nextMeasureDisplay);
        }
        // adds the single note/chord entered by the user to the current measure
        if (isShowCommand == false && userInput[0] != 'q')
        {
            int noteIndex;

            musicSheets.measures[measureIndex].addNote(userInput, isItAChord, false, true, displays[displayIndex]);
            noteIndex = musicSheets.measures[measureIndex].notes.size() - 1;
            // displays the note on the current measure if the user entered a valid note
            if (noteIndex >= 0)
            {
                // adds the note to a chord vector in case the note is part of a chord
                musicSheets.measures[measureIndex].notes[noteIndex].chord.push_back(
                    musicSheets.measures[measureIndex].notes[noteIndex]);
                // sets the chord on/off setting
                displays[displayIndex].setIsItAChord(isItAChord);
                cout << "\n\n" << displays[displayIndex] << "\n\n";
            }
            // user adds notes to a chord 
            while (isItAChord && noteIndex >= 0)
            {
                isShowCommand = false;
                cin >> userInput;
                // indicates that the chord is finished
                if (userInput == "ch" || userInput == "n" || userInput == "q")
                    isItAChord = false;
                // displays all measures
                if (userInput == "show")
                {
                    isShowCommand = true;
                    for (int loopCounter = 0; (size_t)loopCounter < displays.size(); ++loopCounter)
                    {
                        // ensures that the chord on/off setting is not shown
                        displays[loopCounter].setisShowCommand(isShowCommand);
                        cout << "\n\n" << displays[loopCounter] << "\n";
                        displays[loopCounter].setisShowCommand(false);
                    }
                }
                if (isShowCommand == false && isItAChord)
                {
                    musicSheets.measures[measureIndex].notes[noteIndex].chord.push_back(
                        musicSheets.measures[measureIndex].addNote(userInput, isItAChord, true, false,
                            displays[displayIndex]));
                    // removes the note if it is invalid. No matter what the reason is, an invalid note is not assigned a 
                    // frequency 
                    if (musicSheets.measures[measureIndex].notes[noteIndex].chord.back().getNoteFrequency() == 0)
                        musicSheets.measures[measureIndex].notes[noteIndex].chord.pop_back();
                    cout << "\n\n" << displays[displayIndex] << "\n\n";
                }
            }
            if (userInput == "n")
                // indicates that the chord is not finished
                isItAChord = true;
        }
    }

    vector<Note> allNotes;
    int sampleRate = musicSheets.measures[0].getSampleRate();
    Waveform soundData;

    // collects all the notes from the composition
    for (int loopCounter = 0; (size_t)loopCounter < musicSheets.measures.size(); ++loopCounter)
    {
        for (int loopCntr = 0; (size_t)loopCntr < musicSheets.measures[loopCounter].notes.size(); ++loopCntr)
            allNotes.push_back(musicSheets.measures[loopCounter].notes[loopCntr]);
    }
    cout << "Creating song... ";

    // gathers data for audio file
    vector<int16_t> samples = soundData.collectSamples(allNotes, sampleRate);

    path filePath = current_path(); // working directory

    filePath /= "Songs\\music.wav";

    fstream musicFile(filePath.c_str(), ios_base::binary | ios_base::out | ios_base::trunc);

    if (!musicFile.is_open())
    {
        cerr << "WAV file failed to open." << "\n\n";
        exit(EXIT_FAILURE);
    }

    // writes data to audio file 
    WAV musicFileData((uint32_t)sampleRate, samples, musicFile);

    musicFile.close();
    cout << "Done.\n";

    // plays audio file
    try
    {
        // gives the calling thread access to the COM, Component Object Model, library. The COINIT enumeration sets the 
        // calling thread's concurrency model. Setting the parameter to COINIT_APARTMENTTHREADED would create a 
        // single-threaded apartment and COM objects in the apartment's thread would only be able to receive method calls 
        // from that thread. Consequently, multiple instances of a class could be created in different apartments, resulting 
        // in undesirable behavior. COM objects in a multithreaded apartment can receive method calls from any of the 
        // apartment's threads. The COM objects provided by the Windows headers are thread-safe.  
        HRESULT hResult = CoInitializeEx(NULL, COINIT_MULTITHREADED);
        if (!SUCCEEDED(hResult))
            throw runtime_error("Failed to initialize COM library.");

        const int TRACKS = 20;

        BSTR absFilePath = SysAllocString(filePath.c_str());
        CComPtr<IWMPPlayer> playerSPtr;
        CComPtr<IWMPControls> controlsSPtr;
        CComPtr<IWMPMediaCollection> collectionSPtr;
        CComPtr<IWMPPlaylist> playlistSPtr;
        CComPtr<IWMPMedia> musicSPtr, mediaEx1SPtr, mediaEx2SPtr, mediaEx3SPtr, mediaEx4SPtr, mediaEx5SPtr, mediaEx6SPtr;
        CComPtr<IWMPMedia> mediaEx7SPtr, mediaEx8SPtr, mediaEx9SPtr, mediaEx10SPtr, mediaEx11SPtr, mediaEx12SPtr;
        CComPtr<IWMPMedia> mediaEx13SPtr, mediaEx14SPtr, mediaEx15SPtr, mediaEx16SPtr, mediaEx17SPtr, mediaEx18SPtr;
        CComPtr<IWMPMedia> mediaEx19SPtr, mediaEx20SPtr;
        CComPtr<IWMPMedia> mediaFiles[] = { mediaEx1SPtr, mediaEx2SPtr, mediaEx3SPtr, mediaEx4SPtr, mediaEx5SPtr, 
            mediaEx6SPtr, mediaEx7SPtr, mediaEx8SPtr, mediaEx9SPtr, mediaEx10SPtr, mediaEx11SPtr, mediaEx12SPtr, 
            mediaEx13SPtr, mediaEx14SPtr, mediaEx15SPtr, mediaEx16SPtr, mediaEx17SPtr, mediaEx18SPtr, mediaEx19SPtr, 
            mediaEx20SPtr };
        int trackNumber;
       
        // creates an instance of a coclass (Component Object Class). A coclass contains the code for an interface, which is 
        // simply a group of functions. Although the same description can be given for any class object, COM objects also 
        // provide a means of interacting with objects external to a program, such as Windows Media Player. Each coclass has 
        // a CLSID, a unique identifier that is a 128-bit number. The _uuidof operator retrieves the CLSID for the 
        // Windows Media Player coclass. CLSCTX_INPROC_SERVER specifies that the coclass will be managed by a server running 
        // in the same process as the caller of this function, i.e. the code calling the method. 
        hResult = playerSPtr.CoCreateInstance(__uuidof(WindowsMediaPlayer), NULL, CLSCTX_INPROC_SERVER);
        if (SUCCEEDED(hResult))
        {
            // retrieves a pointer to an IWMPControls interface. COM object now has access to the controls of 
            // Windows Media Player 
            hResult = playerSPtr->get_controls(&controlsSPtr);
        }
        else
            throw runtime_error("Failed to create the COM player component.");
        if (SUCCEEDED(hResult))
        {
            // retrieves a pointer to an IWMPMediaCollection interface. COM object now has access to the media collection 
            // of Windows Media Player
            hResult = playerSPtr->get_mediaCollection(&collectionSPtr);
        }
        else
            throw runtime_error("Failed to retrieve pointer to controls interface.");
        if (SUCCEEDED(hResult))
        {
            // retrieves a pointer to an IWMPPlaylist interface. COM object now has access to a playlist in 
            // Windows Media Player
            hResult = playerSPtr->get_currentPlaylist(&playlistSPtr);
        }
        else
            throw runtime_error("Failed to retrieve pointer to media collection interface.");
        if (SUCCEEDED(hResult))
        {
            // adds the user's composition to the media collection
            hResult = collectionSPtr->add(absFilePath, &musicSPtr);
        }
        else
            throw runtime_error("Failed to retrieve pointer to playlist interface.");
        if (SUCCEEDED(hResult))
        {
            for (int loopCounter = 0; loopCounter < TRACKS; ++loopCounter)
            {
                const int BASE_TEN = 10;
                const int INTEGER_TO_ASCII = 48;

                string trackNum;

                // converts integer into its char representation. Code works correctly for single- and double-digit numbers   
                if (loopCounter + 1 >= BASE_TEN)
                {
                    trackNumber = (loopCounter + 1) / BASE_TEN + INTEGER_TO_ASCII;
                    trackNum.push_back((char)trackNumber);
                    trackNumber = (loopCounter + 1) % BASE_TEN + INTEGER_TO_ASCII;
                    trackNum.push_back((char)trackNumber);
                }
                else
                    trackNum.push_back((char)(loopCounter + 1 + INTEGER_TO_ASCII));

                string errorMessage = "Failed to add example  to media collection";
                vector<string> fileNames = { "ex_1.wav", "ex_2.wav", "ex_3.wav", "ex_4.wav", "ex_5.wav", "ex_6.wav",
                    "ex_7.wav", "ex_8.wav", "ex_9.wav", "ex_10.wav", "ex_11.wav", "ex_12.wav", "ex_13.wav", "ex_14.wav",
                    "ex_15.wav", "ex_16.wav", "ex_17.wav", "ex_18.wav", "ex_19.wav", "ex_20.wav" };

                errorMessage.insert(22, trackNum);
                filePath.remove_filename();
                filePath /= fileNames[loopCounter];
                absFilePath = SysAllocString(filePath.c_str());
                // adds an example track to the media collection
                hResult = collectionSPtr->add(absFilePath, &mediaFiles[loopCounter]);
                if (!SUCCEEDED(hResult))
                {
                    throw runtime_error(errorMessage);
                }
            }
            // adds the user's composition to the current playlist
            hResult = playlistSPtr->appendItem(musicSPtr);
        }
        else
            throw runtime_error("Failed to add song to media collection.");
        if (!SUCCEEDED(hResult))
        {
            throw runtime_error("Failed to add song to playlist.");
        }

        bool replaySong = true;

        // plays the user's composition. The user can replay the song and stop it while it is playing
        while (replaySong)
        {
            hResult = controlsSPtr->playItem(musicSPtr);
            if (!SUCCEEDED(hResult))
                throw runtime_error("Failed to play file.");
            cout << "Press Enter to end song.";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            while (cin.get() != '\n');
            hResult = controlsSPtr->stop();
            userInput = " ";
            if (!SUCCEEDED(hResult))
                throw runtime_error("Failed to stop file.");
            while (userInput != "y" && userInput != "n")
            {
                cout << "Replay? (y/n) ";
                cin >> userInput;
                if (userInput == "y")
                    replaySong = true;
                else if (userInput == "n")
                    replaySong = false;
            }
        }
        // removes the user's composition from the media collection 
        hResult = collectionSPtr->remove(musicSPtr, true);
        if (!SUCCEEDED(hResult))
        {
            throw runtime_error("Failed to remove file from media collection.");
        }
        cout << "\n";

        // plays the example track selected by the user. The user can replay the song and stop it while it is playing. 
        // When a new track is selected, the old track is removed from the current playlist. This ensures that only a 
        // single track will play for each user selection
        while (userInput != "q")
        {
            const size_t SHOW_STR_SIZE = 5;

            string showSubstr1 = " ";
            int trackNumber;

            cout << "Play example song? (1-20) ";
            cin >> userInput;
            if (userInput.size() >= SHOW_STR_SIZE)
                showSubstr1 = userInput.substr(0, 4);
            // displays the names of the example tracks
            if (userInput == "show")
                cout << "\n1.  London Bridge Is Falling Down\n2.  Row, Row, Row Your Boat\n3.  Bingo\n"
                    "4.  Twinkle, Twinkle, Little Star\n5.  Oh Where, Oh Where, Has My Little Dog Gone?\n"
                    "6.  Hickory Dickory Dock\n7.  Itsy Bitsy Spider\n8.  Take Me Out To The Ball Game\n"
                    "9.  Old MacDonald Had A Farm\n10. You Are My Sunshine\n11. Humpty Dumpty\n12. Ring A Ring O' Roses\n"
                    "13. Pop! Goes The Weasel\n14. For He's A Jolly Good Fellow\n15. Down By The Bay\n"
                    "16. Head, Shoulders, Knees, And Toes\n17. Five Little Monkeys\n"
                    "18. Here We Go Round The Mulberry Bush\n19. Rain Rain Go Away\n20. I'm A Little Teapot\n\n";
            // displays the name of the selected track
            else if (showSubstr1 == "show")
            {
                string showSubstr2 = userInput.substr(4, 2);
                vector<string> exampleTracks = { "London Bridge Is Falling Down", "Row, Row, Row Your Boat", "Bingo",
                    "Twinkle, Twinkle, Little Star", "Oh Where, Oh Where, Has My Little Dog Gone?", "Hickory Dickory Dock",
                    "Itsy Bitsy Spider", "Take Me Out To The Ball Game", "Old MacDonald Had A Farm", "You Are My Sunshine",
                    "Humpty Dumpty", "Ring A Ring O' Roses", "Pop! Goes The Weasel", "For He's A Jolly Good Fellow",
                    "Down By The Bay", "Head, Shoulders, Knees, And Toes", "Five Little Monkeys",
                    "Here We Go Round The Mulberry Bush", "Rain Rain Go Away", "I'm A Little Teapot" };

                trackNumber = convertCharToInt(showSubstr2);
                if (trackNumber > 0 && trackNumber <= TRACKS)
                    cout << "\n" << exampleTracks[trackNumber - 1] << "\n\n";
            }
            else if (userInput != "q")
            {          
                trackNumber = convertCharToInt(userInput);

                if (trackNumber > 0 && trackNumber <= TRACKS)
                {
                    hResult = playlistSPtr->appendItem(mediaFiles[trackNumber - 1]);
                    if (!SUCCEEDED(hResult))
                        throw runtime_error("Failed to add file to playlist.");
                    hResult = controlsSPtr->playItem(mediaFiles[trackNumber - 1]);
                    if (!SUCCEEDED(hResult))
                        throw runtime_error("Failed to play file.");
                    cout << "Press Enter to end song.";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    while (cin.get() != '\n');
                    hResult = controlsSPtr->stop();
                    if (!SUCCEEDED(hResult))
                        throw runtime_error("Failed to stop file.");
                    hResult = playlistSPtr->removeItem(mediaFiles[trackNumber - 1]);
                    if (!SUCCEEDED(hResult))
                    {
                        throw runtime_error("Failed to remove file from playlist.");
                    }
                }
            }
        }
        // removes the example tracks from the media collection
        for (int loopCounter = 0; loopCounter < TRACKS; ++loopCounter)
        {
            hResult = collectionSPtr->remove(mediaFiles[loopCounter], true);
        }
        if (!SUCCEEDED(hResult))
        {
            throw runtime_error("Failed to remove examples from media collection.");
        }
    }
    catch(runtime_error &exception)
    {
        cerr << exception.what() << "\n\n";
    }
    cout << "\n";

    // closes the COM library
    CoUninitialize();

    cout << "Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    while (cin.get() != '\n');
   
    return(EXIT_SUCCESS);
}
