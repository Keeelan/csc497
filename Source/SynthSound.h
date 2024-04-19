/*
  ==============================================================================

    SynthSound.h
    Created: 28 Feb 2024 1:40:16pm
    Author:  Keelan Siemens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
public:
    bool appliesToNote (int midiNoteNumber) {return true;}
    bool appliesToChannel (int midiChannel) {return true;}
};
