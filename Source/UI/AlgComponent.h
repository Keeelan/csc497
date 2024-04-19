/*
  ==============================================================================

    AlgComponent.h
    Created: 10 Apr 2024 1:45:53pm
    Author:  Keelan Siemens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "CustomComponent.h"

//==============================================================================
/*
*/
class AlgComponent  : public CustomComponent
{
public:
    AlgComponent (juce::AudioProcessorValueTreeState& apvts, juce::String algId);
    ~AlgComponent() override;

    void resized() override;

private:
    juce::ComboBox algSelector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> algSelectorAttachment;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AlgComponent)
};
