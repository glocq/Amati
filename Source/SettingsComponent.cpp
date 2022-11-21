/*
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

#include "SettingsComponent.h"

SettingsComponent::SettingsComponent(juce::ValueTree settingsTree) :
    backendComboBox(settingsTree.getPropertyAsValue("backend", nullptr), "Backend", {"LLVM", "Interpreter"}),
    testComboBox(settingsTree.getPropertyAsValue("test", nullptr), "Test", {"A", "B"})
{
  addAndMakeVisible(backendComboBox);
  // addAndMakeVisible(testComboBox);
}

void SettingsComponent::resized() {
  using FB = juce::FlexBox;
  FB box;
  box.alignContent = FB::AlignContent::flexStart;
  box.alignItems = FB::AlignItems::flexStart;
  box.flexDirection = FB::Direction::column;
  box.flexWrap = FB::Wrap::noWrap;
  box.justifyContent = FB::JustifyContent::flexStart;

  auto addItem = [&](auto& component) {
    auto item = juce::FlexItem(component).withMargin(10).withMaxHeight(60).withMinWidth(getLocalBounds().getWidth()-20).withFlex(1);
    box.items.add(item);
  };
  addItem(backendComboBox);
  // addItem(testComboBox);

  box.performLayout(getLocalBounds());
}

ComboBoxSetting::ComboBoxSetting(const juce::Value &value,
                                 juce::String labelText,
                                 const std::vector<juce::String> &items) {
    label.setText(labelText, juce::dontSendNotification);
    label.attachToComponent(&comboBox, false);
    addAndMakeVisible(label);

    auto id = 1;
    for (const auto& item : items) {
        comboBox.addItem(item, id);
        ++id;
    }
    addAndMakeVisible(comboBox);

    comboBox.getSelectedIdAsValue().referTo(value);
    if (auto var = value.getValue(); var.isVoid() || static_cast<int>(var) < 1 || static_cast<int>(var) > id) {
        comboBox.setSelectedId(1, juce::sendNotification);
    }
}

void ComboBoxSetting::resized() {
  auto bounds = getLocalBounds();
  bounds.removeFromTop(label.getHeight());
  comboBox.setBounds(bounds);
}
