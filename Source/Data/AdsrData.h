/*
  ==============================================================================

    AdsrData.h
    Created: 29 Feb 2024 12:27:18pm
    Author:  Keelan Siemens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class AdsrData  : public juce::ADSR
{
public:
    void update (const float attack, const float decay, const float sustain, const float release);

private:
    juce::ADSR::Parameters adsrParams;
    
    
};
