/*
  ==============================================================================

    GainComponent.cpp
    Created: 9 Apr 2024 7:09:55pm
    Author:  Keelan Siemens

  ==============================================================================
*/

#include <JuceHeader.h>
#include "GainComponent.h"

//==============================================================================
GainComponent::GainComponent (juce::AudioProcessorValueTreeState& apvts, juce::String gainId)
: gain ("dB", gainId, apvts, sliderWidth, sliderHeight, juce::Slider::SliderStyle::LinearVertical)
{

    addAndMakeVisible (gain);
}

GainComponent::~GainComponent()
{
}

void GainComponent::resized()
{
    const auto startX = 33;
    const auto startY = 55;
    const auto width = sliderWidth;
    const auto height = sliderHeight + 20;
    
    gain.setBounds (startX, startY, width, height);
}
