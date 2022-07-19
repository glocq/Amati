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

ParamEditor::ParamEditor ()
{
    for (int i = 0; i < PARAM_COUNT; ++i)
    {
        auto* slider = new juce::Slider();
        auto* label = new juce::Label();
        label->attachToComponent(slider, false);
        label->setText("Label", juce::dontSendNotification);
        sliders.add(slider);
        labels.add(label);

        sliders[i] -> setRange(0.0, 1.0);
        addChildComponent(sliders[i]);
        addChildComponent(labels[i]);
    }
}

void ParamEditor::resized ()
{
    int margin = 50;
    int sliderHeight = 30;
    int sideMargin = 10;

    for (int i = 0; i < PARAM_COUNT; ++i)
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

void ParamEditor::setValue (int index, double value)
{
    sliders[index] -> setValue (value);
}

void ParamEditor::startListeningToSliders (juce::Slider::Listener* listener)
{
    for (int i = 0; i < PARAM_COUNT; ++i)
    {
        sliders[i] -> addListener (listener);
    }
}

bool ParamEditor::compareWithSlider (juce::Slider* slider, int i)
{
    return (slider == sliders[i]);
}

void ParamEditor::updateParameters (AmatiAudioProcessor& processor)
{
    int count = processor.getParamCount ();


    if (count > PARAM_COUNT)
    {
        juce::Logger::getCurrentLogger () -> outputDebugString
        (
            "Number of Faust parameters (" + juce::String (count) +
            ") exceeds maximum number (" + juce::String (PARAM_COUNT) + ") of parameters!"
        );

        count = PARAM_COUNT;
    }

    for (int i = 0; i < count; ++i)
    {
        sliders[i]->setValue (0.0);
        sliders[i]->setVisible (true);
        labels[i]->setVisible(true);
        labels[i]->setText(processor.getLabel(i), juce::dontSendNotification);
    }

    for (int i = count; i < PARAM_COUNT; ++i)
    {
        sliders[i]->setVisible(false);
        labels[i]->setVisible(false);
    }
}

void ParamEditor::updateParameterValues (AmatiAudioProcessor& processor)
{
    int count = processor.getParamCount ();
    count = count <= PARAM_COUNT ? count : PARAM_COUNT;

    for (int i = 0; i < count; ++i)
    {
        sliders[i] -> setValue (processor.getParameterValue (i));
    }
}

