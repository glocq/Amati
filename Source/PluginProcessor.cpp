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

static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout() {
  juce::AudioProcessorValueTreeState::ParameterLayout layout;
  for (int i = 0; i < PARAM_COUNT; i++) {
    auto id = juce::ParameterID(paramIdForIdx(i), 1);
    auto name = juce::String("Parameter ") + juce::String(i);
    layout.add(std::make_unique<juce::AudioParameterFloat>(id, name, 0.f, 1.f, 0.f));
  }
  return layout;
}

AmatiAudioProcessor::AmatiAudioProcessor() :
#ifndef JucePlugin_PreferredChannelConfigurations
     AudioProcessor (BusesProperties()
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                       ),
#endif
      valueTreeState(*this, nullptr, "parameters", createParameterLayout())
{}

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

void AmatiAudioProcessor::setCurrentProgram (int /* index */)
{
}

const juce::String AmatiAudioProcessor::getProgramName (int /* index */)
{
    return {};
}

void AmatiAudioProcessor::changeProgramName (int /* index */, const juce::String& /* newName */)
{
}

void AmatiAudioProcessor::prepareToPlay (double sampRate, int samplesPerBlock)
{
    sampleRate = sampRate;

    // numChannelsIn and numChannelsOut should be equal
    // (and probably equal to 0),
    // but we get both just in case

    int numChannelsIn  = tmpBufferIn.getNumChannels ();
    int numChannelsOut = tmpBufferOut.getNumChannels ();

    tmpBufferIn  = juce::AudioBuffer<float> (numChannelsIn,  samplesPerBlock);
    tmpBufferOut = juce::AudioBuffer<float> (numChannelsOut, samplesPerBlock);
    compileSource(sourceCode);
    playing = true;
}

void AmatiAudioProcessor::releaseResources()
{
  faustProgram.reset(nullptr);
  playing = false;
}

bool AmatiAudioProcessor::isBusesLayoutSupported (const BusesLayout&) const
{
    // We want to accept any layout; the responsibility is on the user to write
    // a Faust program that is compatible with the layout
    return true;
}

void AmatiAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& /* midiMessages */)
{
    updateDspParameters();

    int numSamples = buffer.getNumSamples ();

    // The host should not give us more samples than expected.
    // If it does though, we resize our internal buffers
    if (numSamples > tmpBufferIn.getNumSamples ())
    {
        tmpBufferIn.setSize  (tmpBufferIn.getNumChannels  (), numSamples);
        tmpBufferOut.setSize (tmpBufferOut.getNumChannels (), numSamples);
    }

    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    if (not faustProgram)
    {
        for (auto i = 0; i < totalNumOutputChannels; ++i)
            buffer.clear (i, 0, numSamples);
    }
    else
    {
        // What is going to happen:
        // buffer will be copied into tmpBufferIn. tmpBufferIn will be processed into tmpBufferOut.
        // Then tmpBufferOut will be copied into buffer again.
        // This is to maks sure we do the computation in a controlled environment
        // (i.e. buffers with known number of channels).
        // I hope all those copies won't take up too much time though...

        for (int chan = 0; (chan < totalNumInputChannels) && (chan < tmpBufferIn.getNumChannels ()); ++chan)
            tmpBufferIn.copyFrom (chan, 0, buffer, chan, 0, numSamples);

        // Clear remaining channels, if any
        for (int chan = totalNumInputChannels; chan < tmpBufferIn.getNumChannels (); ++ chan)
            tmpBufferIn.clear (chan, 0, numSamples);

        // ---

        faustProgram->compute(numSamples, tmpBufferIn.getArrayOfReadPointers(), tmpBufferOut.getArrayOfWritePointers());

        // ---

        for (int chan = 0; (chan < totalNumOutputChannels) && (chan < tmpBufferOut.getNumChannels ()); ++chan)
            buffer.copyFrom (chan, 0, tmpBufferOut, chan, 0, numSamples);

        // Clear remaining channels, if any
        for (int chan = tmpBufferOut.getNumChannels (); chan < totalNumOutputChannels; ++chan)
            buffer.clear (chan, 0, numSamples);
    }
}

bool AmatiAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* AmatiAudioProcessor::createEditor()
{
    return new AmatiAudioProcessorEditor (*this, valueTreeState);
}

void AmatiAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::Logger::getCurrentLogger()->writeToLog ("Saving preset");

    // Create parent element
    juce::XmlElement preset = juce::XmlElement ("amati_preset");

    // Add source code. TODO allow for source code on file
    juce::XmlElement* sourceTag = preset.createNewChildElement ("source");
    sourceTag->addTextElement(sourceCode);

    auto state = valueTreeState.copyState().createXml();
    preset.addChildElement(new juce::XmlElement(*state.get()));

    // We're done! We can store our tree on disk now
    copyXmlToBinary (preset, destData);
}

void AmatiAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    juce::Logger::getCurrentLogger() -> writeToLog ("Loading preset");

    std::unique_ptr<juce::XmlElement> preset (getXmlFromBinary (data, sizeInBytes));

    if (preset.get())
    {
        juce::XmlElement* parameters = preset->getChildByName(valueTreeState.state.getType());
        if (parameters) {
          valueTreeState.replaceState(juce::ValueTree::fromXml(*parameters));
          sourceCode = preset -> getChildByName ("source") -> getAllSubText ();
          if (playing) {
            compileSource (sourceCode);
          }
        } else {
          juce::Logger::getCurrentLogger()->writeToLog("Invalid preset");
        }

    }
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new AmatiAudioProcessor();
}

bool AmatiAudioProcessor::compileSource (juce::String source)
{
  switch (backend) {
  case FaustProgram::Backend::LLVM:
    juce::Logger::getCurrentLogger()->writeToLog ("Compiling with LLVM backend...");
    break;
  case FaustProgram::Backend::Interpreter:
    juce::Logger::getCurrentLogger()->writeToLog ("Compiling with Interpreter backend...");
    break;
  }
  try {
      faustProgram.reset(new FaustProgram(source, backend, sampleRate));
      juce::Logger::getCurrentLogger()->writeToLog ("Compilation complete! Using new program.");
    } catch (FaustProgram::CompileError& e) {
      auto* logger = juce::Logger::getCurrentLogger();
      logger->writeToLog ("Compilation failed!");
      logger->writeToLog (e.what());
      return false;
    }

    sourceCode = source;

    // Update internal buffers
    int inChans  = faustProgram->getNumInChannels  ();
    int outChans = faustProgram->getNumOutChannels ();

    tmpBufferIn.setSize  (inChans,  tmpBufferIn.getNumSamples  ());
    tmpBufferOut.setSize (outChans, tmpBufferOut.getNumSamples ());
    return true;
}

juce::String AmatiAudioProcessor::getSourceCode ()
{
    return sourceCode;
}

int AmatiAudioProcessor::getParamCount ()
{
    return faustProgram->getParamCount();
}

juce::String AmatiAudioProcessor::getLabel(size_t idx) {
  return faustProgram->getLabel(idx);
}

void AmatiAudioProcessor::setParameter (int index, float value)
{
    controlParameters[index]->setValueNotifyingHost (value);
}

void AmatiAudioProcessor::updateDspParameters ()
{
    size_t count = faustProgram->getParamCount();
    for (size_t i = 0; i < count; ++i) {
        faustProgram->setValue (i, *valueTreeState.getRawParameterValue(paramIdForIdx(i)));
    }
}

FaustProgram::ItemType AmatiAudioProcessor::getType(size_t idx) {
  return faustProgram->getType(idx);
}

void AmatiAudioProcessor::setBackend(FaustProgram::Backend newBackend) {
  backend = newBackend;
}