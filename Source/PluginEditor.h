/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/AdsrComponent.h"
#include "UI/GainComponent.h"
#include "UI/OscComponent.h"
#include "UI/LfoComponent.h"
#include "UI/AlgComponent.h"

//==============================================================================
/**
*/
class KeelanSynthAudioProcessorEditor  : public juce::AudioProcessorEditor
, public juce::Timer
{
public:
    KeelanSynthAudioProcessorEditor (KeelanSynthAudioProcessor&);
    ~KeelanSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    void timerCallback() override;
    
private:

    KeelanSynthAudioProcessor& audioProcessor;
    OscComponent osc1;
    OscComponent osc2;
    OscComponent osc3;
    OscComponent osc4;
    AlgComponent alg;
    AdsrComponent adsr;
    GainComponent gain;
    LfoComponent lfo1;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeelanSynthAudioProcessorEditor)
};
