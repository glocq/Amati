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

#include "PluginProcessor.h"

class AmatiSliderParameterAttachment : private juce::Slider::Listener
{
public:
  AmatiSliderParameterAttachment (juce::RangedAudioParameter& parameter, juce::Slider& slider,
                                  juce::UndoManager* undoManager = nullptr);

  ~AmatiSliderParameterAttachment() override;

  void sendInitialUpdate();

private:
  void setValue (float newValue);
  void sliderValueChanged (juce::Slider*) override;

  void sliderDragStarted (juce::Slider*) override { attachment.beginGesture(); }
  void sliderDragEnded   (juce::Slider*) override { attachment.endGesture(); }

  juce::Slider& slider;
  juce::ParameterAttachment attachment;
  bool ignoreCallbacks = false;
};

class AmatiSliderAttachment
{
public:
  AmatiSliderAttachment (AudioProcessorValueTreeState& stateToUse,
                         const String& parameterID,
                         Slider& slider
  );

private:
  std::unique_ptr<AmatiSliderParameterAttachment> attachment;
  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmatiSliderAttachment)
};


class ParamEditor : public juce::Component
{
public:
    ParamEditor (juce::AudioProcessorValueTreeState&);
    ~ParamEditor () noexcept override;

    void paint (juce::Graphics&) override {}
    void resized () override;

    using Param = AmatiAudioProcessor::FaustParameter;

    void updateParameters(const std::vector<Param>&);

private:
    juce::AudioProcessorValueTreeState& valueTreeState;
    juce::OwnedArray<juce::Component> components{};
    juce::OwnedArray<juce::Label> labels{};
    juce::OwnedArray<AmatiSliderAttachment> sliderAttachments{};
    juce::OwnedArray<juce::AudioProcessorValueTreeState::ButtonAttachment> buttonAttachments{};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParamEditor)
};
