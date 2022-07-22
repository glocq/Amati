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

// Use the Interprter backend instead of the LLVM backend
//#define INTERP

#include <JuceHeader.h>
#ifdef INTERP
#include <faust/dsp/interpreter-dsp.h>
#else
#include <faust/dsp/llvm-dsp.h>
#endif
#include <faust/gui/APIUI.h>

// Wrapper around Faust-related stuff, namely:
// - a Faust program
// - the associated user interface

class FaustProgram
{

public:
  enum class ItemType {
      // Unavailabe is for when we don't/can't include the UI element;
      // As of now, only sliders are available (and they are all horizontal)
      Unavailable,
      Slider,
      Button,
    };

    FaustProgram (int sampRate);
    ~FaustProgram ();

    bool compileSource (juce::String);
    size_t getParamCount ();
    int getNumInChannels ();
    int getNumOutChannels ();

    // Getters for values associated with a parameter
    // (UI element type; minimum, maximum and initial value)
    // Argument has to be less than parameter count!
    ItemType getType (size_t);
    double getMin (size_t);
    double getMax (size_t);
    double getInit (size_t);

    float getValue (size_t);
    void setValue (size_t, float);
    juce::String getLabel(size_t idx);

    void compute(int sampleCount, const float** input, float** output);
    void setSampleRate (int);
    bool isReady ();

private:

    dsp_factory* dspFactory = nullptr;
    dsp* dspInstance = nullptr;
    APIUI* faustInterface = nullptr;
    bool ready = false;
    int sampleRate;

};
