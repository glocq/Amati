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

#include "ParamEditor.h"

ParamEditor::ParamEditor (juce::AudioProcessorValueTreeState& vts) :
  valueTreeState(vts) {}

ParamEditor::~ParamEditor() noexcept {
  sliderAttachments.clear();
  buttonAttachments.clear();
  labels.clear();
  components.clear();
}

void ParamEditor::updateParameters(const std::vector<Param>& params) {
  sliderAttachments.clear();
  buttonAttachments.clear();
  labels.clear();
  components.clear();

  for (auto& p : params) {
    switch (p.type) {
      case Param::Type::Slider: {
        auto *slider = new juce::Slider();
        slider->setRange(0.0, 1.0);

        auto *attachment = new juce::AudioProcessorValueTreeState::SliderAttachment(
            valueTreeState, p.id, *slider);

        auto *label = new juce::Label();
        label->attachToComponent(slider, false);
        label->setText(p.label, juce::dontSendNotification);

        components.add(slider);
        labels.add(label);
        sliderAttachments.add(attachment);

        addAndMakeVisible(slider);
        addAndMakeVisible(label);
        break;
      }
      case Param::Type::Button: {
        auto *button = new juce::TextButton(p.label);
        button->setClickingTogglesState (true);
        auto *attachment = new juce::AudioProcessorValueTreeState::ButtonAttachment(
            valueTreeState, p.id, *button);
        components.add(button);
        buttonAttachments.add(attachment);

        addAndMakeVisible(button);
        break;
      }
    }
  }

  resized();
}

void ParamEditor::resized ()
{
    int margin = 50;
    int sliderHeight = 30;
    int sideMargin = 10;

    for (int i = 0; i < components.size(); ++i)
    {
        components[i]->setBounds(
            sideMargin,
            (1+i) * margin + i * sliderHeight,
            getWidth () - sideMargin*2,
            sliderHeight
        );
    }
}