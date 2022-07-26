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

#pragma once

#include <JuceHeader.h>
#include <faust/dsp/llvm-dsp.h>

#include "FaustProgram.h"

inline juce::String paramIdForIdx(int idx) {
  return juce::String("Param") + juce::String(idx);
}
inline juce::String paramIdForIdx(size_t idx) {
  return paramIdForIdx(static_cast<int>(idx));
}

//==============================================================================
/**
*/
class AmatiAudioProcessor  : public juce::AudioProcessor, juce::ValueTree::Listener
{
public:
    //==============================================================================
    AmatiAudioProcessor();
    ~AmatiAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

  private:
    void valueTreePropertyChanged(ValueTree &treeWhosePropertyHasChanged,
                                  const Identifier &property) override;

  public:
    //==============================================================================
    /**
        Compile the source code given as a string.
        If compilation is successful, use the resulting program,
        and set the processor's internal source code to be the new code.
    */
    bool compileSource (juce::String);
    juce::String getSourceCode ();
    void setBackend(FaustProgram::Backend);

    int getParamCount ();
    juce::String getLabel(size_t idx);
    FaustProgram::ItemType getType(size_t idx);

    struct FaustParameter {
      juce::String id;
      juce::String label;
      enum class Type {
        Slider,
        Button,
      };
      Type type;
    };
    std::vector<FaustParameter> getFaustParameters() const;
private:
    //==============================================================================
    // We keep a copy of the source code inside the processor.
    // The GUI's code editor will refer to it.
    juce::String sourceCode = "";
    FaustProgram::Backend backend{};
    std::unique_ptr<FaustProgram> faustProgram{};
    bool playing{false};

    juce::AudioProcessorValueTreeState valueTreeState;

    // Used in processBlock to copy input and output buffers
    juce::AudioBuffer<float> tmpBufferIn;
    juce::AudioBuffer<float> tmpBufferOut;

    double sampleRate;

    void updateDspParameters ();

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmatiAudioProcessor)
};
