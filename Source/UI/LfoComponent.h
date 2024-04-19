/*
  ==============================================================================

    LfoComponent.h
    Created: 8 Apr 2024 5:54:42pm
    Author:  Keelan Siemens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CustomComponent.h"

//==============================================================================
/*
*/
class LfoComponent  : public CustomComponent
{
public:
    LfoComponent (juce::AudioProcessorValueTreeState& apvts, juce::String lfoFreqId, juce::String lfoPmdId, juce::String lfoAmdId, juce::String lfoWaveTypeId);
    ~LfoComponent() override;

    void resized() override;

private:
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;

    void setSliderParams (juce::Slider& slider, juce::Label& label, std::unique_ptr<SliderAttachment>& attachment, juce::String paramId, juce::AudioProcessorValueTreeState& apvts);
    
    juce::Slider lfoFreqSlider;
    juce::Slider lfoPmdSlider;
    juce::Slider lfoAmdfSlider;
    
    juce::Label lfoFreqLabel { "Freq", "Freq" };
    juce::Label lfoPmdLabel { "PMD", "PMD" };
    juce::Label lfoAmdLabel { "AMD", "AMD" };
    
    std::unique_ptr<SliderAttachment> lfoFreqAttachment;
    std::unique_ptr<SliderAttachment> lfoPmdAttachment;
    std::unique_ptr<SliderAttachment> lfoAmdAttachment;
    
    SliderWithLabel lfoFreq;
    SliderWithLabel lfoPmd;
    SliderWithLabel lfoAmd;
    
    juce::ComboBox waveSelector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> waveSelectorAttachment;
    
    static constexpr int dialWidth = 70;
    static constexpr int dialHeight = 70;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (LfoComponent)
};
