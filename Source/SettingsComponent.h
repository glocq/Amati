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
#pragma once

#include <juce_gui_basics/juce_gui_basics.h>

class ComboBoxSetting : public juce::Component {
public:
  ComboBoxSetting(const juce::Value& value, juce::String labelText, const std::vector<juce::String>& items);

  void paint (juce::Graphics&) override {}
  void resized () override;

private:
  juce::Label label;
  juce::ComboBox comboBox;
};

class SettingsComponent : public juce::Component {
public:
  SettingsComponent(juce::ValueTree settingsTree);

  void paint (juce::Graphics&) override {}
  void resized () override;

private:
  ComboBoxSetting backendComboBox;
  ComboBoxSetting testComboBox;
};
