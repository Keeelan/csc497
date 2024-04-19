/*
  ==============================================================================

    OscComponent.h
    Created: 29 Feb 2024 1:28:11pm
    Author:  Keelan Siemens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CustomComponent.h"

//==============================================================================
/*
*/
class OscComponent  : public CustomComponent
{
public:
    OscComponent (juce::AudioProcessorValueTreeState& apvts, juce::String oscId, juce::String fmRatioId, juce::String fmDepthId, juce::String feedbackId);
    ~OscComponent() override;
    
    void resized() override;

private:
    juce::ComboBox oscWaveSelector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> oscWaveSelectorAttachment;
       
    SliderWithLabel fmRatio;
    SliderWithLabel fmDepth;
    FeedbackSlider feedback;
    
    static constexpr int dialWidth = 70;
    static constexpr int dialHeight = 70;
    static constexpr int sliderWidth = 100;
    static constexpr int sliderHeight = 50;
    
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscComponent)
};
