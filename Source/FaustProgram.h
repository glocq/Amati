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


// Wrapper around Faust-related stuff, namely:
// - a Faust program
// - the associated user interface

class FaustProgram
{

private:
    // UNAVAILABLE is for when we don't/can't include the UI element;
    // As of now, only sliders are available (and they are all horizontal)
    enum ItemType {UNAVAILABLE, SLIDER};

public:

    FaustProgram (int sampRate);
    ~FaustProgram ();

    bool compileSource (juce::String);
    int getParamCount ();
    int getNumInChannels ();
    int getNumOutChannels ();

    // Getters for values associated with a parameter
    // (UI element type; minimum, maximum and initial value)
    // Argument has to be less than parameter count!
    ItemType getType (int);
    double getMin (int);
    double getMax (int);
    double getInit (int);

    double getValue (int);
    void setValue (int, double);
    juce::String getLabel(int idx);

    void compute (int sampleCount, float** input, float** output);
    void setSampleRate (int);
    bool isReady ();

private:

    llvm_dsp* dspInstance = nullptr;
    APIUI* faustInterface = nullptr;
    bool ready = false;
    int sampleRate;


};
