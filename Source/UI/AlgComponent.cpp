/*
  ==============================================================================

    AlgComponent.cpp
    Created: 10 Apr 2024 1:45:53pm
    Author:  Keelan Siemens

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AlgComponent.h"


//==============================================================================
AlgComponent::AlgComponent (juce::AudioProcessorValueTreeState& apvts, juce::String algId)
{
    juce::StringArray choices {"1", "2", "3", "4", "5", "6", "7", "8"};
    algSelector.addItemList(choices, 1);
    addAndMakeVisible (algSelector);
    algSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, algId, algSelector);
}

AlgComponent::~AlgComponent()
{
}

void AlgComponent::resized()
{
    algSelector.setBounds (18, 40, 100, 25);
}

