/*
  ==============================================================================

    OscComponent.cpp
    Created: 29 Feb 2024 1:28:11pm
    Author:  Keelan Siemens

  ==============================================================================
*/

#include <JuceHeader.h>
#include "OscComponent.h"

//==============================================================================
OscComponent::OscComponent (juce::AudioProcessorValueTreeState& apvts, juce::String oscId, juce::String fmRatioId, juce::String fmDepthId, juce::String feedbackId)
: fmRatio ("FM Ratio", fmRatioId, apvts, dialWidth, dialHeight)
, fmDepth ("FM Depth", fmDepthId, apvts, dialWidth, dialHeight)
, feedback ("Feedback:", feedbackId, apvts, sliderWidth, sliderHeight, juce::Slider::SliderStyle::LinearHorizontal)
{
    juce::StringArray choices {"Sine", "Saw", "Square"};
    oscWaveSelector.addItemList(choices, 1);
    addAndMakeVisible (oscWaveSelector);
    
    oscWaveSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, oscId, oscWaveSelector);
    
    addAndMakeVisible (fmRatio);
    addAndMakeVisible (fmDepth);
    if (oscId == "OSC4"){
        
        addAndMakeVisible(feedback);
    }

}

OscComponent::~OscComponent()
{
}

void OscComponent::resized()
{
    const auto yStart = 15;
    const auto width = 70;
    const auto height = 88;
    
    oscWaveSelector.setBounds (18, 40, 100, 25);
    feedback.setBounds (18, oscWaveSelector.getBottom() + 10, sliderWidth, sliderHeight);
    fmRatio.setBounds (120, yStart, width, height);
    fmDepth.setBounds (190, yStart, width, height);
}
