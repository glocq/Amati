/*
    Copyright (C) 2020 by Grégoire Locqueville <gregoireloc@gmail.com>
    Copyright (C) 2022 by Kamil Kisiel <kamil@kamilkisiel.net>

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
#include "FaustCodeTokenizer.h"


class EditorComponent :
    public juce::Component
{
public:
    EditorComponent ();

    void paint (juce::Graphics&) override {}
    void resized () override;

    juce::String getSource ();
    void setSource (juce::String);

    std::function<void(void)> onCompile;
    std::function<void(void)> onRevert;

private:
    FaustTokeniser tokeniser;

    // Warning: sourceCode has to be declared before codeEditor!
    // Don't swap the following two lines.
    juce::CodeDocument sourceCode;
    juce::CodeEditorComponent codeEditor;

    juce::TextButton compileButton;
    juce::TextButton revertButton;
    juce::TextButton importButton;
    juce::TextButton exportButton;

    std::unique_ptr<juce::FileChooser> fileChooser;
    juce::File workdir; // TODO: Store this in settings so it persists between runs.

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EditorComponent)
};
