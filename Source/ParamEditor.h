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


class ParamEditor : public juce::Component
{
public:
    struct Param {
      juce::String id;
      juce::String label;
    };
    ParamEditor (juce::AudioProcessorValueTreeState&);
    ~ParamEditor () noexcept override;

    void paint (juce::Graphics&) override {}
    void resized () override;
    void updateParameters(const std::vector<Param>&);

private:
    juce::AudioProcessorValueTreeState& valueTreeState;
    juce::OwnedArray<juce::Slider> sliders{};
    juce::OwnedArray<juce::Label> labels{};
    juce::OwnedArray<juce::AudioProcessorValueTreeState::SliderAttachment> attachments{};

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParamEditor)
};
