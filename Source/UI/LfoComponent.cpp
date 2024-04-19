/*
  ==============================================================================

    LfoComponent.cpp
    Created: 8 Apr 2024 5:54:42pm
    Author:  Keelan Siemens

  ==============================================================================
*/

#include <JuceHeader.h>
#include "LfoComponent.h"

//==============================================================================
LfoComponent::LfoComponent (juce::AudioProcessorValueTreeState& apvts, juce::String lfoFreqId, juce::String lfoPmdId, juce::String lfoAmdId, juce::String lfoWaveTypeId)
: lfoFreq ("LFO Freq", lfoFreqId, apvts, dialWidth, dialHeight)
, lfoPmd ("LFO PMD", lfoPmdId, apvts, dialWidth, dialHeight)
, lfoAmd ("LFO AMD", lfoAmdId, apvts, dialWidth, dialHeight)
{
    juce::StringArray choices { "Saw", "Square", "Triangle", "Sample / Hold" };
    waveSelector.addItemList(choices, 1);
    addAndMakeVisible (waveSelector);
    waveSelectorAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(apvts, lfoWaveTypeId, waveSelector);
    addAndMakeVisible (lfoFreq);
    addAndMakeVisible (lfoPmd);
    addAndMakeVisible (lfoAmd);
}

LfoComponent::~LfoComponent()
{
}

void LfoComponent::resized()
{
    const auto width = 70;
    const auto height = 88;
    
    waveSelector.setBounds (18, 40, 100, 25);
    lfoFreq.setBounds (53, waveSelector.getBottom() + 5, width, height);
    lfoPmd.setBounds (18, lfoFreq.getBottom(), width, height);
    lfoAmd.setBounds (90, lfoFreq.getBottom(), width, height);
}

