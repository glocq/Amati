/*
    Copyright (C) 2020 by Grégoire Locqueville <gregoireloc@gmail.com>

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

    const char* argv[] = {}; // compilation arguments
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

        faustInterface = new MapUI;
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

double FaustProgram::getValue (int index)
{
    if (index < 0 || index >= getParamCount ())
        return 0.0;
    else
        return (faustInterface->getParamValue (faustInterface->getParamAddress (index)));
}

void FaustProgram::setValue (int index, double value)
{
    if (index < 0 || index >= getParamCount ()) {}
    else
        faustInterface->setParamValue (faustInterface->getParamAddress (index), value);
}

void FaustProgram::compute (int samples, float** in, float** out)
{
    dspInstance -> compute (samples, in, out);
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
