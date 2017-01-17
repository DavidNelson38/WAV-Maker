#ifndef MUSIC_WAV_H
#define MUSIC_WAV_H

#include <fstream>
using std::fstream;

#include <vector>
using std::vector;

namespace Music
{   
    class WAV
    {
    public:
        WAV(unsigned int sampleRate, vector<short> samples, fstream &fileStream);
    private:
        char *ID;
        unsigned int fileLength;
        char *riffType;
        char *chunkMarker;
        unsigned int chunkSize;
        unsigned short formatMethod;
        unsigned short channels;
        unsigned int sampleRate;
        unsigned int bytesPerSecond;
        unsigned short blockAlignment;
        unsigned short bitDepth;
        char *dataMarker;
        unsigned int dataSize;
        vector<short> samples;
    };
}

#endif
