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

#include "ConsoleComponent.h"
#include "EditorComponent.h"
#include "ParamEditor.h"
#include "PluginProcessor.h"
#include "SettingsComponent.h"

#include <JuceHeader.h>
#include <faust/gui/MapUI.h>

class AmatiAudioProcessorEditor :
    public juce::AudioProcessorEditor,
    public juce::Button::Listener,
    public juce::ValueTree::Listener
{
public:
    AmatiAudioProcessorEditor (AmatiAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~AmatiAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

    //==============================================================================
    void buttonClicked (juce::Button*) override;

private:
  AmatiAudioProcessor& audioProcessor;
  juce::AudioProcessorValueTreeState& valueTreeState;
  juce::ValueTree settingsTree;

  MapUI faustUI;

    void updateParameters ();
    void updateEditor ();

    juce::TabbedComponent tabbedComponent;
    EditorComponent editorComponent;
    ParamEditor paramEditor;
    ConsoleComponent consoleTab;
    SettingsComponent settingsComponent;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AmatiAudioProcessorEditor)
};
