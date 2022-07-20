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
  attachments.clear();
  labels.clear();
  sliders.clear();
}

void ParamEditor::updateParameters(const std::vector<Param>& params) {
  attachments.clear();
  labels.clear();
  sliders.clear();

  for (auto& p : params) {
    auto *slider = new juce::Slider();
    slider->setRange(0.0, 1.0);

    auto *attachment = new juce::AudioProcessorValueTreeState::SliderAttachment(
        valueTreeState, p.id, *slider);

    auto *label = new juce::Label();
    label->attachToComponent(slider, false);
    label->setText(p.label, juce::dontSendNotification);

    sliders.add(slider);
    labels.add(label);
    attachments.add(attachment);

    addAndMakeVisible(slider);
    addAndMakeVisible(label);
  }

  resized();
}

void ParamEditor::resized ()
{
    int margin = 50;
    int sliderHeight = 30;
    int sideMargin = 10;

    for (int i = 0; i < sliders.size(); ++i)
    {
        sliders[i] -> setBounds
        (
            sideMargin,
            (1+i) * margin + i * sliderHeight,
            getWidth () - sideMargin,
            sliderHeight
        );
    }
}