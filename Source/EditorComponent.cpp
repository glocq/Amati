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

#include "EditorComponent.h"

EditorComponent::EditorComponent () :
    codeEditor (sourceCode, nullptr)
{
    addAndMakeVisible (&codeEditor);

    compileButton.setComponentID ("compile");
    compileButton.setButtonText ("Compile");
    addAndMakeVisible (&compileButton);
}

void EditorComponent::resized ()
{
    int margin = 10;
    int buttonHeight = 30;
    int buttonWidth = 100;

    compileButton.setBounds
    (
        margin,
        margin,
        buttonWidth,
        buttonHeight
    );

    codeEditor.setBounds
    (
        margin,
        2*margin + buttonHeight,
        getWidth() - 2*margin,
        getHeight() - 3*margin - buttonHeight
    );
}

void EditorComponent::startListeningToCompileButton (juce::Button::Listener* listener)
{
    compileButton.addListener (listener);
}

juce::String EditorComponent::getSource ()
{
    return sourceCode.getAllContent ();
}

void EditorComponent::setSource (juce::String source)
{
    sourceCode.replaceAllContent (source);
}

