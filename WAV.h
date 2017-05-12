// WAV.h
//
// Format for a WAV file.  
//

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
        WAV(uint32_t sampleRate, vector<int16_t> &samples, fstream &fileStream);
    private:
        uint8_t *headerID;        // header identifier  
        uint32_t fileLength;      // file size in bytes, does not include headerID and fileLength 
        uint8_t *riffType;        // type of container format
        uint8_t *formatID;        // format chunk identifier 
        uint32_t formatSize;      // size of format chunk in bytes, does not include formatID and formatSize 
        uint16_t formatMethod;    // method used to represent analog signals
        uint16_t channels;        // number of channels (mono - one channel, stereo - two or more channels)
        //uint32_t sampleRate;    // frames per second
        uint32_t bytesPerSecond;  // bytes per second
        uint16_t blockAlignment;  // number of bytes in a frame, value used for buffer alignment
        uint16_t bitDepth;        // number of bits in a sample
        uint8_t *dataID;          // data chunk identifier
        uint32_t dataSize;        // size of the samples in bytes
        //vector<int16_t> samples // contains the samples
    };
}

#endif
