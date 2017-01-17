#include "Frequencies.h"
using Music::Frequency;

Music::Frequency::Frequency()
{
    scientific["c0"] = 0; scientific["d0"] = 2; scientific["e0"] = 4; scientific["f0"] = 5; scientific["g0"] = 7; 
    scientific["a0"] = 9; scientific["b0"] = 11; scientific["c1"] = 12; scientific["d1"] = 14; scientific["e1"] = 16; 
    scientific["f1"] = 17; scientific["g1"] = 19; scientific["a1"] = 21; scientific["b1"] = 23; scientific["c2"] = 24; 
    scientific["d2"] = 26; scientific["e2"] = 28; scientific["f2"] = 29; scientific["g2"] = 31; scientific["a2"] = 33; 
    scientific["b2"] = 35; scientific["c3"] = 36; scientific["d3"] = 38; scientific["e3"] = 40; scientific["f3"] = 41; 
    scientific["g3"] = 43; scientific["a3"] = 45; scientific["b3"] = 47; scientific["c4"] = 48; scientific["d4"] = 50; 
    scientific["e4"] = 52; scientific["f4"] = 53; scientific["g4"] = 55; scientific["a4"] = 57; scientific["b4"] = 59;
    scientific["c5"] = 60; scientific["d5"] = 62; scientific["e5"] = 64; scientific["f5"] = 65; scientific["g5"] = 67;  
    scientific["a5"] = 69; scientific["b5"] = 71; scientific["c6"] = 72; scientific["d6"] = 74; scientific["e6"] = 76; 
    scientific["f6"] = 77; scientific["g6"] = 79; scientific["a6"] = 81; scientific["b6"] = 83; scientific["c7"] = 84; 
    scientific["d7"] = 86; scientific["e7"] = 88; scientific["f7"] = 89; scientific["g7"] = 91; scientific["a7"] = 93; 
    scientific["b7"] = 95; scientific["c8"] = 96; scientific["d8"] = 98; scientific["e8"] = 100; scientific["f8"] = 101;  
    scientific["g8"] = 103; scientific["a8"] = 105; scientific["b8"] = 107; scientific["c9"] = 108; scientific["d9"] = 110; 
    scientific["e9"] = 112; scientific["f9"] = 113; scientific["g9"] = 115; scientific["a9"] = 117; scientific["b9"] = 119;

    frequencies[0] = 16.35; frequencies[1] = 17.32; frequencies[2] = 18.35; frequencies[3] = 19.45; 
    frequencies[4] = 20.60; frequencies[5] = 21.83; frequencies[6] = 23.12; frequencies[7] = 24.50; 
    frequencies[8] = 25.96; frequencies[9] = 27.50; frequencies[10] = 29.14; frequencies[11] = 30.87; 
    frequencies[12] = 32.70; frequencies[13] = 34.65; frequencies[14] = 36.71; frequencies[15] = 38.89; 
    frequencies[16] = 41.20; frequencies[17] = 43.65; frequencies[18] = 46.25; frequencies[19] = 49.00; 
    frequencies[20] = 51.91; frequencies[21] = 55.00; frequencies[22] = 58.27; frequencies[23] = 61.74; 
    frequencies[24] = 65.41; frequencies[25] = 69.30; frequencies[26] = 73.42; frequencies[27] = 77.78;
    frequencies[28] = 82.41; frequencies[29] = 87.31; frequencies[30] = 92.50; frequencies[31] = 98.00; 
    frequencies[32] = 103.83; frequencies[33] = 110.00; frequencies[34] = 116.54; frequencies[35] = 123.47;
    frequencies[36] = 130.81; frequencies[37] = 138.59; frequencies[38] = 146.83; frequencies[39] = 155.56;
    frequencies[40] = 164.81; frequencies[41] = 174.61; frequencies[42] = 185.00; frequencies[43] = 196.00;
    frequencies[44] = 207.65; frequencies[45] = 220.00; frequencies[46] = 233.08; frequencies[47] = 246.94;
    frequencies[48] = 261.63; frequencies[49] = 277.18; frequencies[50] = 293.66; frequencies[51] = 311.13;
    frequencies[52] = 329.63; frequencies[53] = 349.23; frequencies[54] = 369.99; frequencies[55] = 392.00;
    frequencies[56] = 415.30; frequencies[57] = 440.00; frequencies[58] = 466.16; frequencies[59] = 493.88;
    frequencies[60] = 523.25; frequencies[61] = 554.37; frequencies[62] = 587.33; frequencies[63] = 622.25;
    frequencies[64] = 659.25; frequencies[65] = 698.46; frequencies[66] = 739.99; frequencies[67] = 783.99; 
    frequencies[68] = 830.61; frequencies[69] = 880.00; frequencies[70] = 932.33; frequencies[71] = 987.77;
    frequencies[72] = 1046.50; frequencies[73] = 1108.73; frequencies[74] = 1174.66; frequencies[75] = 1244.51;
    frequencies[76] = 1318.51; frequencies[77] = 1396.91; frequencies[78] = 1479.98; frequencies[79] = 1567.98;
    frequencies[80] = 1661.22; frequencies[81] = 1760.00; frequencies[82] = 1864.66; frequencies[83] = 1975.53;
    frequencies[84] = 2093.00; frequencies[85] = 2217.46; frequencies[86] = 2349.32; frequencies[87] = 2489.02;
    frequencies[88] = 2637.02; frequencies[89] = 2793.83; frequencies[90] = 2959.96; frequencies[91] = 3135.96;
    frequencies[92] = 3322.44; frequencies[93] = 3520.00; frequencies[94] = 3729.31; frequencies[95] = 3951.07;
    frequencies[96] = 4186.01; frequencies[97] = 4434.92; frequencies[98] = 4698.63; frequencies[99] = 4978.03;
    frequencies[100] = 5274.04; frequencies[101] = 5587.65; frequencies[102] = 5919.91; frequencies[103] = 6271.93;
    frequencies[104] = 6644.88; frequencies[105] = 7040.00; frequencies[106] = 7458.62; frequencies[107] = 7902.13;
    frequencies[108] = 8372.02; frequencies[109] = 8869.84; frequencies[110] = 9397.27; frequencies[111] = 9956.06;
    frequencies[112] = 10548.08; frequencies[113] = 11175.30; frequencies[114] = 11839.82; frequencies[115] = 12543.86;
    frequencies[116] = 13289.75; frequencies[117] = 14080.00; frequencies[118] = 14917.24; frequencies[119] = 15804.26;
}

map<string, int> *
Music::Frequency::getScientific() 
{
    map<string, int> *scientificPtr = &scientific;

    return (scientificPtr);
}

map<int, double> *
Music::Frequency::getFrequencies()
{
    map<int, double> *frequencyPtr = &frequencies;

    return(frequencyPtr);
}
