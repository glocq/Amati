/*
    Copyright (C) 2020, 2021 by Grégoire Locqueville <gregoireloc@gmail.com>

    This file is part of Amati.

    Amati is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Amati is distributed in the hope that it will be useful, but
    WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Amati.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <JuceHeader.h>

#include <faust/dsp/llvm-dsp.h>
#include <faust/gui/APIUI.h>


class FaustProgram
{
public:

    FaustProgram (int sampRate);
    ~FaustProgram ();

    bool compileSource (juce::String);
    int getParamCount ();
    int getNumInChannels ();
    int getNumOutChannels ();

    double getValue (int);
    void setValue (int, double);

    void compute (int sampleCount, float** input, float** output);
    void setSampleRate (int);
    bool isReady ();

private:

    llvm_dsp* dspInstance = nullptr;
    APIUI* faustInterface = nullptr;
    bool ready = false;
    int sampleRate;
};

