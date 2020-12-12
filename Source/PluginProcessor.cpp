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

#include "PluginProcessor.h"

#include "PluginEditor.h"


AmatiAudioProcessor::AmatiAudioProcessor() :
#ifndef JucePlugin_PreferredChannelConfigurations
     AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
     faustProgram (44100)
{
    for (int i = 0; i < PARAM_COUNT; ++i)
    {
        juce::AudioParameterFloat* newParam = new juce::AudioParameterFloat
        (
            "param" + juce::String (i),
            "Parameter " + juce::String (i),
            0.0f,
            1.0f,
            0.0f
        );

        controlParameters.insert (i, newParam);
        addParameter (newParam);
    }
}

AmatiAudioProcessor::~AmatiAudioProcessor()
{
}

const juce::String AmatiAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool AmatiAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool AmatiAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool AmatiAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double AmatiAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int AmatiAudioProcessor::getNumPrograms()
{
    return 1;
}

int AmatiAudioProcessor::getCurrentProgram()
{
    return 0;
}

void AmatiAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String AmatiAudioProcessor::getProgramName (int index)
{
    return {};
}

void AmatiAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

void AmatiAudioProcessor::prepareToPlay (double sampRate, int samplesPerBlock)
{
    sampleRate = sampRate;
    tmpBuffer = juce::AudioBuffer<float> (getTotalNumInputChannels (), samplesPerBlock);
}

void AmatiAudioProcessor::releaseResources()
{
}

bool AmatiAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    // We want to accept any layout; the responsibility is on the user to write
    // a Faust program that is compatible with the layout
    return true;
}

void AmatiAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    updateDspParameters ();

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    if (not faustProgram.isReady ())
    {
        for (auto i = 0; i < totalNumOutputChannels; ++i)
            buffer.clear (i, 0, buffer.getNumSamples ());
    }
    else
    {
        // Copy the input buffer
        tmpBuffer.makeCopyOf (buffer);

        // Clear all output channels, now that we've copied their contents.
        // This is probably useless, but we're doing it just in case.
        for (auto i = 0; i < totalNumOutputChannels; ++i)
            buffer.clear (i, 0, buffer.getNumSamples());

        // It doesn't seem to be a problem if we try to write to more channels than we're allowed.
        // TODO Investigate why I can't use a read-only pointer to get the input
        faustProgram.compute
        (
            buffer.getNumSamples (),
            tmpBuffer.getArrayOfWritePointers (),
            buffer.getArrayOfWritePointers ()
        );
    }
}

bool AmatiAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* AmatiAudioProcessor::createEditor()
{
    return new AmatiAudioProcessorEditor (*this);
}

void AmatiAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::Logger::getCurrentLogger() -> writeToLog ("Saving preset");

    // Create parent element
    juce::XmlElement preset = juce::XmlElement ("amati_preset");

    // Add source code. TODO allow for source code on file
    juce::XmlElement* sourceTag = preset.createNewChildElement ("source");
    sourceTag -> addTextElement (sourceCode);

    // Add values for the parameters

    juce::XmlElement* parameters = preset.createNewChildElement ("parameters");

    for (int i = 0; i < getParamCount (); ++i)
    {
        juce::XmlElement* slot = parameters -> createNewChildElement ("slot");
        slot -> setAttribute ("index", i);
        slot -> setAttribute ("value", getParameterValue (i));
    }

    // We're done! We can store our tree on disk now
    copyXmlToBinary (preset, destData);
}

void AmatiAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::Logger::getCurrentLogger() -> writeToLog ("Loading preset");

    std::unique_ptr<juce::XmlElement> preset (getXmlFromBinary (data, sizeInBytes));

    if (preset.get())
    {
        sourceCode = preset -> getChildByName ("source") -> getAllSubText ();
        compileSource (sourceCode);

        juce::XmlElement* parameters = preset -> getChildByName ("parameters");

        if (parameters)
        {
            juce::XmlElement* child = parameters -> getFirstChildElement ();

            while (child)
            {
                int    index = child -> getIntAttribute    ("index", -1);
                double value = child -> getDoubleAttribute ("value");

                if (index != -1)
                {
                    setParameter (index, value);
                }
                else
                    juce::Logger::getCurrentLogger() -> writeToLog ("Preset parameter missing index");

                child = child -> getNextElement ();
            }
        }
        else
            juce::Logger::getCurrentLogger() -> writeToLog ("Invalid preset");
    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AmatiAudioProcessor();
}

bool AmatiAudioProcessor::compileSource (juce::String source)
{
    bool result = faustProgram.compileSource (source);

    if (result)
        sourceCode = source;

    return result;
}

juce::String AmatiAudioProcessor::getSourceCode ()
{
    return sourceCode;
}

int AmatiAudioProcessor::getParamCount ()
{
    return faustProgram.getParamCount ();
}

double AmatiAudioProcessor::getParameterValue (int index)
{
    return (controlParameters[index] -> get ());
}

void AmatiAudioProcessor::setParameter (int index, double value)
{
    controlParameters[index] -> setValueNotifyingHost (value);
}

void AmatiAudioProcessor::beginGesture (int index)
{
    controlParameters[index] -> beginChangeGesture ();
}

void AmatiAudioProcessor::endGesture (int index)
{
    controlParameters[index] -> endChangeGesture ();
}


void AmatiAudioProcessor::updateDspParameters ()
{
    int count = faustProgram.getParamCount ();

    for (int i = 0; i < count; ++i)
    {
        double val = controlParameters[i] -> get(); 
        faustProgram.setValue (i, val);
    }
}

