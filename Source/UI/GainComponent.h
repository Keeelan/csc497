/*
  ==============================================================================

    GainComponent.h
    Created: 9 Apr 2024 7:09:55pm
    Author:  Keelan Siemens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CustomComponent.h"

//==============================================================================
/*
*/
class GainComponent  : public CustomComponent
{
public:
    GainComponent (juce::AudioProcessorValueTreeState& apvts, juce::String gainId);
    ~GainComponent() override;
    void resized() override;

private:
    SliderWithLabel gain;
    
    static constexpr int sliderWidth = 50;
    static constexpr int sliderHeight = 430;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainComponent)
};
