/*
  ==============================================================================

    AdsrComponent.h
    Created: 29 Feb 2024 12:26:56pm
    Author:  Keelan Siemens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CustomComponent.h"

//==============================================================================
/*
*/
class AdsrComponent  : public CustomComponent
{
public:
    AdsrComponent (juce::AudioProcessorValueTreeState& apvts, juce::String attackId, juce::String decayId, juce::String sustainId, juce::String releaseId);
    ~AdsrComponent() override;
//    void paint (juce::Graphics&) override;
    void resized() override;

private:
    SliderWithLabel attack;
    SliderWithLabel decay;
    SliderWithLabel sustain;
    SliderWithLabel release;
    
    static constexpr int sliderWidth = 50;
    static constexpr int sliderHeight = 430;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AdsrComponent)
};
