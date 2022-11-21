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

#include "EditorComponent.h"

EditorComponent::EditorComponent () :
    codeEditor (sourceCode, &tokeniser),
                                     workdir(File::getSpecialLocation (File::userHomeDirectory))
{
    addAndMakeVisible (&codeEditor);

    compileButton.setComponentID ("compile");
    compileButton.setButtonText ("Compile");
    compileButton.onClick = [this] {
      if (onCompile) {
        onCompile();
      }
    };
    addAndMakeVisible (&compileButton);

    importButton.setButtonText("Import...");
    importButton.onClick = [this] {
      fileChooser = std::make_unique<juce::FileChooser> ("Please select the DSP file you want to load...", workdir, "*.dsp");

      auto chooserFlags = juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles;

      fileChooser->launchAsync (chooserFlags, [this] (const FileChooser& chooser)
      {
        auto sourceFile = chooser.getResult();
        setSource (sourceFile.loadFileAsString());
        workdir = sourceFile.getParentDirectory();
      });
    };
    addAndMakeVisible(importButton);

    exportButton.setButtonText("Export...");
    exportButton.onClick = [this] {
      fileChooser = std::make_unique<juce::FileChooser> ("Select where to save DSP file...", workdir, "*.dsp");

      auto chooserFlags = juce::FileBrowserComponent::saveMode | juce::FileBrowserComponent::canSelectFiles;
      fileChooser->launchAsync (chooserFlags, [this] (const FileChooser& chooser) {
        auto sourceFile = chooser.getResult();
        auto outputStream = juce::FileOutputStream(sourceFile);
        sourceCode.writeToStream(outputStream);
        workdir = sourceFile.getParentDirectory();
      });
    };
    addAndMakeVisible(exportButton);

    revertButton.setButtonText("Revert");
    addAndMakeVisible(revertButton);
    revertButton.onClick = [this] {
        if (onRevert) {
            onRevert();
        }
    };

}

void EditorComponent::resized ()
{
    int margin = 10;
    int buttonHeight = 30;
    int buttonWidth = 100;

    using FB = juce::FlexBox;
    FB buttons;
    buttons.flexDirection = FB::Direction::row;
    buttons.alignItems = FB::AlignItems::flexStart;
    buttons.justifyContent = FB::JustifyContent::flexStart;
    buttons.flexWrap = FB::Wrap::noWrap;
    buttons.alignContent = FB::AlignContent::flexStart;

    auto addButton = [&](auto& button) {
      buttons.items.add(juce::FlexItem(button).withMargin(margin).withMinHeight(buttonHeight).withMaxWidth(buttonWidth).withFlex(1));
    };
    addButton(compileButton);
    addButton(importButton);
    addButton(exportButton);
    addButton(revertButton);

    auto bounds = getLocalBounds();
    buttons.performLayout(bounds.removeFromTop(buttonHeight));

    codeEditor.setBounds
    (
        margin,
        2*margin + buttonHeight,
        getWidth() - 2*margin,
        getHeight() - 3*margin - buttonHeight
    );
}

juce::String EditorComponent::getSource ()
{
    return sourceCode.getAllContent ();
}

void EditorComponent::setSource (juce::String source)
{
    sourceCode.replaceAllContent (source);
}

