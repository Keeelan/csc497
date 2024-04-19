/*
  ==============================================================================

    OscData.h
    Created: 29 Feb 2024 1:08:58pm
    Author:  Keelan Siemens

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class OscData : public juce::dsp::Oscillator<float>
{
public:
    void prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels);
    void setWaveType (const int choice);
    void setOscPitch (const int pitch);
    void setFreq (const int midiNoteNumber);
    void setFmOsc (const float ratio, const float depth, const float ratio1, const float depth1, const float ratio2, const float depth2, const float ratio3, const float depth3);
    float processNextSample (float input, float lfoValue);
    void setParams (const int oscChoice, const int oscPitch, const float oscRatio, const float oscDepth, const float fm1Ratio, const float fm1Depth, const float fm2Ratio, const float fm2Depth, const float fm3Ratio, const float fm3Depth, const float fm3Feedback, const float alg);
    void resetAll();
private:
    
    juce::dsp::Oscillator<float> fm1Osc { [](float x) { return std::sin (x); }};
    juce::dsp::Oscillator<float> fm2Osc { [](float x) { return std::sin (x); }};
    juce::dsp::Oscillator<float> fm3Osc { [](float x) { return std::sin (x); }};
    
    int lastPitch { 0 };
    float oscDepth { 0.0f };
    float fm1Depth { 0.0f };
    float fm2Depth { 0.0f };
    float fm3Depth { 0.0f };
    
    float oscRatio { 0.0f };
    float fm1Ratio { 0.0f };
    float fm2Ratio { 0.0f };
    float fm3Ratio { 0.0f };
    
    float fmMod { 0.0f };
    float fm1Mod { 0.0f };
    float fm2Mod { 0.0f };
    float fm3Mod { 0.0f };
    
    int algorithm { 1 };
    float feedback { 0.00f };
    
    int lastMidiNote { 0 };
    float lastSampleFeedbackOsc { 0.00f };
};
