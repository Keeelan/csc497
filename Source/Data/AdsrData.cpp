/*
  ==============================================================================

    AdsrData.cpp
    Created: 29 Feb 2024 12:27:18pm
    Author:  Keelan Siemens

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AdsrData.h"

//==============================================================================
void AdsrData::update (const float attack, const float decay, const float sustain, const float release)
{
    adsrParams.attack = attack;
    adsrParams.decay = decay;
    adsrParams.sustain = sustain;
    adsrParams.release = release;
    
    setParameters(adsrParams);
}
