// Frequencies.h
//
// Maps note frequencies to scientific pitch notation. 
//

#ifndef MUSIC_FREQUENCIES_H
#define MUSIC_FREQUENCIES_H

#include <map>
using std::map;

#include <string>
using std::string;

namespace Music
{
    class Frequency
    {
    public:
        Frequency();
        map<string, int> *getScientific();
        map<int, double> *getFrequencies();
    private:
        map<string, int> scientific;
        map<int, double> frequencies;
    };
}

#endif
