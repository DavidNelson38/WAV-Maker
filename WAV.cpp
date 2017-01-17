#include <cstring>
using std::strcpy;

#include "WAV.h"
using Music::WAV;

#include <iostream>
using std::cout;

Music::WAV::WAV(unsigned int sampleRate, vector<short> samples, fstream &fileStream) : 
    sampleRate(sampleRate), samples(samples)
{
    const int SIZE = 5;
    const unsigned short PCM = 1;
    const unsigned short CHANNELS = 1;
    const unsigned short BIT_DEPTH = 16;
    const unsigned short BITS_IN_BYTE = 8;

    char riff[SIZE] = "RIFF";
    char riffType[SIZE] = "WAVE";
    char fmt[SIZE] = "fmt ";
    char data[SIZE] = "data";
    short dataSample;

    formatMethod = PCM;
    channels = CHANNELS;
    blockAlignment = CHANNELS * BIT_DEPTH / BITS_IN_BYTE;
    bitDepth = BIT_DEPTH;
    bytesPerSecond = this->sampleRate * blockAlignment;
    chunkSize = (unsigned int)(sizeof(formatMethod) + sizeof(channels) + sizeof(blockAlignment) + sizeof(this->sampleRate)
        + sizeof(bitDepth) + sizeof(bytesPerSecond));
    fileLength = (unsigned int)(sizeof(riffType) - sizeof(riffType[0]) + sizeof(fmt) - sizeof(fmt[0]) + sizeof(chunkSize) 
        + sizeof(data) - sizeof(data[0]) + sizeof(dataSize)) + chunkSize + dataSize;

    fileStream.write("RIFF", 4);
    fileStream.write((char *)&fileLength, sizeof(fileLength));
    fileStream.write("WAVE", 4);
    fileStream.write("fmt ", 4);
    fileStream.write((char *)&chunkSize, sizeof(chunkSize));
    fileStream.write((char *)&formatMethod, sizeof(formatMethod));
    fileStream.write((char *)&channels, sizeof(channels));
    fileStream.write((char *)&sampleRate, sizeof(sampleRate));
    fileStream.write((char *)&bytesPerSecond, sizeof(bytesPerSecond));
    fileStream.write((char *)&blockAlignment, sizeof(blockAlignment));
    fileStream.write((char *)&bitDepth, sizeof(bitDepth));
    fileStream.write("data", 4);
    fileStream.write((char *)&dataSize, sizeof(dataSize));
    for (int loopCounter = 0; (size_t)loopCounter < samples.size(); ++loopCounter)
    {
        dataSample = samples[loopCounter];
        fileStream.write((char *)&dataSample, sizeof(dataSample));
    }
}
