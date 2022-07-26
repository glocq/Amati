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

#include <limits>

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
AmatiAudioProcessorEditor::AmatiAudioProcessorEditor (AmatiAudioProcessor& p, juce::AudioProcessorValueTreeState& vts) :
    AudioProcessorEditor (&p),
    audioProcessor (p),
                                                                                                                      valueTreeState(vts),
    tabbedComponent (juce::TabbedButtonBar::TabsAtTop),
    paramEditor(vts),
    settingsTree(vts.state.getOrCreateChildWithName("settings", nullptr)),
    settingsComponent(settingsTree)
{
    // Graphics stuff ----------------------------------------------------------

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);

    addAndMakeVisible (&tabbedComponent);

    juce::Colour tabColour = getLookAndFeel(). findColour (juce::TabbedComponent::backgroundColourId);
    tabbedComponent.addTab ("Editor", tabColour, &editorComponent, false);
    tabbedComponent.addTab ("Parameters", tabColour, &paramEditor, false);
    tabbedComponent.addTab ("Console", tabColour, &consoleTab, false);
    tabbedComponent.addTab("Settings", tabColour, &settingsComponent, false);

    setResizable (true, true);
    // So we have to set a maximum size? Well we'll just use the maximum possible integer
    setResizeLimits (100, 100, std::numeric_limits<int>::max(), std::numeric_limits<int>::max());

    //---------------------------------------------------------------------------

    editorComponent.onCompile = [&] {
      consoleTab.clearMessages();
      if (audioProcessor.compileSource (editorComponent.getSource ()))
      {
        updateParameters ();
        tabbedComponent.setCurrentTabIndex(1);
      } else {
        tabbedComponent.setCurrentTabIndex(2);
      }
    };
    editorComponent.onRevert = [&] {
      updateEditor();
    };

    updateParameters (); // set the right display for the parameters
    updateEditor (); // set editor to display the processor's source code

    juce::Logger::setCurrentLogger (&consoleTab);

    settingsTree.addListener(this);
}

AmatiAudioProcessorEditor::~AmatiAudioProcessorEditor()
{
    juce::Logger::setCurrentLogger (nullptr);
}

void AmatiAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
}

void AmatiAudioProcessorEditor::resized()
{
    int margin = 10;
    tabbedComponent.setBounds (margin, margin, getWidth() - 2*margin, getHeight() - 2*margin);
}

void AmatiAudioProcessorEditor::buttonClicked (juce::Button* button)
{
    juce::String id = button -> getComponentID ();

    if (id == "compile")
    {
   }
}

//==============================================================================
void AmatiAudioProcessorEditor::updateParameters ()
{
  paramEditor.updateParameters(audioProcessor.getFaustParameters());
}

void AmatiAudioProcessorEditor::updateEditor ()
{
    editorComponent.setSource (audioProcessor.getSourceCode ());
}