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

#include "FaustProgram.h"


FaustProgram::FaustProgram (int sampRate) : sampleRate (sampRate)
{
}

FaustProgram::~FaustProgram ()
{
    delete faustInterface;
    delete dspInstance;
}

bool FaustProgram::compileSource (juce::String source)
{
    juce::Logger::getCurrentLogger() -> writeToLog ("Starting compilation...");

    const char* argv[] = {""}; // compilation arguments
    std::string errorString;

    llvm_dsp_factory* factory = createDSPFactoryFromString
    (
        "faust", // program name
        source.toStdString (),
        0, // number of arguments
        argv,
        "", // compilation target; left empty to say we want to compile for this machine
        errorString
    );


    if (factory) // compilation successful!
    {
        llvm_dsp* formerInstance = dspInstance;

        dspInstance = factory -> createDSPInstance ();

        // delete factory; // we won't need this anymore
        delete formerInstance; // has been replaced
        delete faustInterface; // to start fresh

        dspInstance -> init (sampleRate);

        faustInterface = new APIUI;
        dspInstance -> buildUserInterface (faustInterface);

        ready = true;

        juce::Logger::getCurrentLogger() -> writeToLog ("Compilation complete! Using new program.");

        return true;
    }
    else
    {
        juce::Logger::getCurrentLogger() -> writeToLog ("Compilation failed!");
        juce::Logger::getCurrentLogger() -> writeToLog (errorString);

        return ready;
    }
}

int FaustProgram::getParamCount ()
{
    if (faustInterface)
        return faustInterface -> getParamsCount ();
    else
        return 0;
}


int FaustProgram::getNumInChannels ()
{
    if (dspInstance)
        return (dspInstance -> getNumInputs ());
    else
        return 0;
}


int FaustProgram::getNumOutChannels ()
{
    if (dspInstance)
        return (dspInstance -> getNumOutputs ());
    else
        return 0;
}


FaustProgram::ItemType FaustProgram::getType (int index)
{
    APIUI::ItemType type = faustInterface->getParamItemType (index);

    if (type == APIUI::kVSlider || type == APIUI::kVSlider)
        return SLIDER;
    else
        return UNAVAILABLE;
}


double FaustProgram::getMin (int index)
{
    return (faustInterface->getParamMin (index));
}


double FaustProgram::getMax (int index)
{
    return (faustInterface->getParamMax (index));
}


double FaustProgram::getInit (int index)
{
    return (faustInterface->getParamInit (index));
}


double FaustProgram::getValue (int index)
{
    if (index < 0 || index >= getParamCount ())
        return 0.0;
    else
        return (faustInterface->getParamValue (index));
}

void FaustProgram::setValue (int index, double value)
{
    if (index < 0 || index >= getParamCount ()) {}
    else
        faustInterface->setParamValue (index, static_cast<float>(value));
}

void FaustProgram::compute(int samples, const float** in, float** out)
{
    dspInstance -> compute (samples, const_cast<float**>(in), out);
}

void FaustProgram::setSampleRate (int sampRate)
{
    sampleRate = sampRate;
    ready = false;
}

bool FaustProgram::isReady ()
{
    return ready;
}

juce::String FaustProgram::getLabel(int idx) {
  return juce::String(faustInterface->getParamLabel(idx));
}
