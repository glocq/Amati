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
    deleteDSPFactory(dspFactory);
}

bool FaustProgram::compileSource (juce::String source)
{
    juce::Logger::getCurrentLogger() -> writeToLog ("Starting compilation...");

    const char* argv[] = {""}; // compilation arguments
    std::string errorString;
    
    llvm_dsp_factory* formerFactory = dspFactory;

    dspFactory = createDSPFactoryFromString
    (
        "faust", // program name
        source.toStdString (),
        0, // number of arguments
        argv,
        "", // compilation target; left empty to say we want to compile for this machine
        errorString
    );


    if (dspFactory) // compilation successful!
    {
        llvm_dsp* formerInstance = dspInstance;

        dspInstance = dspFactory -> createDSPInstance ();
   
        delete formerInstance; // has been replaced
        deleteDSPFactory(formerFactory);
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
      auto* logger = juce::Logger::getCurrentLogger();
      logger->writeToLog ("Compilation failed!");
      logger->writeToLog (errorString);

        return false;
    }
}

size_t FaustProgram::getParamCount ()
{
    if (faustInterface)
        return static_cast<size_t>(faustInterface->getParamsCount());
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


FaustProgram::ItemType FaustProgram::getType (size_t index)
{
    auto type = faustInterface->getParamItemType(index);
    switch (type) {
    case APIUI::kButton:
    case APIUI::kCheckButton:
      return ItemType::Button;
    case APIUI::kVSlider:
    case APIUI::kHSlider:
    case APIUI::kNumEntry:
      return ItemType::Slider;
    case APIUI::kHBargraph:
    case APIUI::kVBargraph:
    default:
      return ItemType::Unavailable;
    }
}


double FaustProgram::getMin (size_t index)
{
    return (faustInterface->getParamMin (index));
}


double FaustProgram::getMax (size_t index)
{
    return (faustInterface->getParamMax (index));
}


double FaustProgram::getInit (size_t index)
{
    return (faustInterface->getParamInit (index));
}


float FaustProgram::getValue (size_t index)
{
    if (index < 0 || index >= getParamCount ())
        return 0.0;
    else
        return faustInterface->getParamRatio(index);
}

void FaustProgram::setValue (size_t index, float value)
{
    if (index < 0 || index >= getParamCount ()) {}
    else
        faustInterface->setParamRatio(index, value);
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

juce::String FaustProgram::getLabel(size_t idx) {
  return juce::String(faustInterface->getParamLabel(idx));
}
