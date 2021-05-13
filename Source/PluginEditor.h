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
#include <faust/gui/MapUI.h>
#include "PluginProcessor.h"
#include "EditorComponent.h"
#include "ParamEditor.h"
#include "DebugComponent.h"

class AmatiAudioProcessorEditor :
    public juce::AudioProcessorEditor,
    public juce::Button::Listener,
    public juce::Slider::Listener,
    public juce::Timer
{
public:
    AmatiAudioProcessorEditor (AmatiAudioProcessor&);
    ~AmatiAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    //==============================================================================
    void buttonClicked (juce::Button*) override;

    void sliderValueChanged (juce::Slider*) override;
    void sliderDragStarted (juce::Slider*) override;
    void sliderDragEnded (juce::Slider*) override;

    void timerCallback () override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AmatiAudioProcessor& audioProcessor;

    MapUI faustUI;

    void updateParameters ();
    void updateParameterValues ();
    void updateEditor ();

    juce::TabbedComponent tabbedComponent;
    EditorComponent editorComponent;
    ParamEditor paramEditor;
    DebugComponent consoleTab;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmatiAudioProcessorEditor)
};
