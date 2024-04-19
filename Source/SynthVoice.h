/*
  ==============================================================================

    SynthVoice.h
    Created: 28 Feb 2024 1:39:57pm
    Author:  Keelan Siemens

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"
#include "Data/AdsrData.h"
#include "Data/OscData.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels, int lfoType);
    void renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples) override;
    void updateAlgorithm (int algo);
    void updateGain (float gainVal);
    void reset();
    void updateLFO(int pmd, int amd, float freq, int lfoType);
    
    std::array<OscData, 2>& getOscillator1() { return osc1; }
    std::array<OscData, 2>& getOscillator2() { return osc2; }
    std::array<OscData, 2>& getOscillator3() { return osc3; }
    std::array<OscData, 2>& getOscillator4() { return osc4; }
    
    AdsrData& getAdsr() { return adsr; }
                       
private:
    AdsrData adsr;
    
    juce::AudioBuffer<float> synthBuffer;
    juce::AudioBuffer<float> tempBuffer;
    static constexpr int numChannelsToProcess { 2 };
    
    std::array<OscData, numChannelsToProcess> osc1;
    std::array<OscData, numChannelsToProcess> osc2;
    std::array<OscData, numChannelsToProcess> osc3;
    std::array<OscData, numChannelsToProcess> osc4;
    
    std::array<juce::dsp::Oscillator<float>, numChannelsToProcess> lfo;
    float lfoPMD { 0 };
    float lfoAMD { 0 };
    
    juce::dsp::Gain<float> gain;
    int algorithm { 1 };
    
    bool isPrepared { false };
};
