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

#include "ConsoleComponent.h"

ConsoleComponent::ConsoleComponent()
{
    console.setReadOnly (true);
    console.setCaretVisible (false);
    console.setMultiLine (true);

    // Use monospaced font
    juce::Font font;
    font.setTypefaceName (juce::Font::getDefaultMonospacedFontName ());
    console.setFont (font);

    addAndMakeVisible (&console);
}

void ConsoleComponent::resized ()
{
    int margin = 10;

    console.setBounds
    (
        margin,
        margin,
        getWidth  () - 2*margin,
        getHeight () - 2*margin
    );
}

void ConsoleComponent::clearMessages() {
    console.clear();
}

void ConsoleComponent::logMessage (const juce::String& message)
{
    console.setCaretPosition (console.getTotalNumChars ());
    console.insertTextAtCaret (message + "\n");
}

