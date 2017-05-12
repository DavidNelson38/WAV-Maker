// WAV.cpp
//
// Format for a WAV file.  
//

#include "WAV.h"
using Music::WAV;

Music::WAV::WAV(uint32_t sampleRate, vector<int16_t> &samples, fstream &fileStream)
{
    const uint16_t SIZE = 5;
    const uint16_t PCM = 1;          // value indicates pulse-code modulation (PCM) will be used 
    const uint16_t CHANNELS = 1;
    const uint16_t BIT_DEPTH = 16;
    const uint16_t BITS_IN_BYTE = 8;

    uint8_t riff[SIZE] = "RIFF";
    uint8_t riffType[SIZE] = "WAVE";
    uint8_t fmt[SIZE] = "fmt ";
    uint8_t data[SIZE] = "data";

    formatMethod = PCM;
    channels = CHANNELS;
    blockAlignment = CHANNELS * BIT_DEPTH / BITS_IN_BYTE;
    bitDepth = BIT_DEPTH;
    bytesPerSecond = sampleRate * blockAlignment;
    formatSize = (uint32_t)(sizeof(formatMethod) + sizeof(channels) + sizeof(sampleRate) + sizeof(bytesPerSecond) 
        + sizeof(blockAlignment) + sizeof(bitDepth));
    // identifiers need to be exactly four bytes
    fileLength = (uint32_t)(sizeof(riffType) - sizeof(riffType[0]) + sizeof(fmt) - sizeof(fmt[0]) + sizeof(formatSize) 
        + sizeof(data) - sizeof(data[0]) + sizeof(dataSize)) + formatSize + dataSize;

    fileStream.write((char *)riff, 4);
    fileStream.write((char *)&fileLength, sizeof(fileLength));
    fileStream.write((char *)riffType, 4);
    fileStream.write((char *)fmt, 4);
    fileStream.write((char *)&formatSize, sizeof(formatSize));
    fileStream.write((char *)&formatMethod, sizeof(formatMethod));
    fileStream.write((char *)&channels, sizeof(channels));
    fileStream.write((char *)&sampleRate, sizeof(sampleRate));
    fileStream.write((char *)&bytesPerSecond, sizeof(bytesPerSecond));
    fileStream.write((char *)&blockAlignment, sizeof(blockAlignment));
    fileStream.write((char *)&bitDepth, sizeof(bitDepth));
    fileStream.write((char *)data, 4);
    fileStream.write((char *)&dataSize, sizeof(dataSize));
    for (int loopCounter = 0; (size_t)loopCounter < samples.size(); ++loopCounter)
    {
        int16_t dataSample; // a single data point

        dataSample = (int16_t)samples[loopCounter];
        fileStream.write((char *)&dataSample, sizeof(dataSample));
    }
}
