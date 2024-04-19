/*
  ==============================================================================

    SynthVoice.cpp
    Created: 28 Feb 2024 1:39:57pm
    Author:  Keelan Siemens

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound (juce::SynthesiserSound* sound)
{
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition)
{
    for (int i = 0; i < 2; i++)
    {
        osc1[i].setFreq (midiNoteNumber);
        osc2[i].setFreq (midiNoteNumber);
        osc3[i].setFreq (midiNoteNumber);
        osc4[i].setFreq (midiNoteNumber);
    }
    
    adsr.noteOn();
}

void SynthVoice::stopNote (float velocity, bool allowTailOff)
{
    adsr.noteOff();
    
    if (!allowTailOff || !adsr.isActive())
        clearCurrentNote();
}

void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue)
{
    
}

void SynthVoice::pitchWheelMoved (int newPitchWheelValue)
{
    
}

void SynthVoice::prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels, int lfoType)
{
    reset();
    
    adsr.setSampleRate(sampleRate);
    
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
    
    for (int ch = 0; ch < numChannelsToProcess; ch++)
    {
        osc1[ch].prepareToPlay (sampleRate, samplesPerBlock, outputChannels);
        osc2[ch].prepareToPlay (sampleRate, samplesPerBlock, outputChannels);
        osc3[ch].prepareToPlay (sampleRate, samplesPerBlock, outputChannels);
        osc4[ch].prepareToPlay (sampleRate, samplesPerBlock, outputChannels);
        lfo[ch].prepare (spec);
        lfo[ch].initialise ([](float x) { return x / juce::MathConstants<float>::pi; });
    }
    
    gain.prepare (spec);
    gain.setGainLinear (0.3f);
    isPrepared = true;
}

void SynthVoice::renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples)
{
    jassert (isPrepared);
    
    if (!isVoiceActive())
        return;
    
    synthBuffer.setSize (outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer.clear();
    float l = 0.0f;
    for (int ch = 0; ch < synthBuffer.getNumChannels(); ++ch)
    {
        auto* buffer = synthBuffer.getWritePointer (ch, 0);
        
        for (int s = 0; s < synthBuffer.getNumSamples(); ++s)
        {
            l = lfo[ch].processSample(0) * lfoPMD;
            switch (algorithm) {
                case 1:
                case 2:
                case 3:
                case 4:
                    buffer[s] = osc1[ch].processNextSample (0, l);
                    break;
                case 5:
                    buffer[s] = osc1[ch].processNextSample (0, l) + osc3[ch].processNextSample (0, l);
                    break;
                case 6:
                case 7:
                    buffer[s] = osc1[ch].processNextSample (0, l) + osc2[ch].processNextSample (0, l) + osc3[ch].processNextSample (0, l);
                    break;
                case 8:
                    buffer[s] = osc1[ch].processNextSample (0, l) + osc2[ch].processNextSample (0, l) + osc3[ch].processNextSample (0, l) + osc4[ch].processNextSample (0, l);
                    break;
                default:
                    jassertfalse;
                    break;
            }
            buffer[s] = buffer[s] * juce::Decibels::decibelsToGain<float> (lfo[ch].processSample(0) * lfoAMD * 21);
            
        }
    }

    juce::dsp::AudioBlock<float> audioBlock { synthBuffer };
    gain.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
    adsr.applyEnvelopeToBuffer (synthBuffer, 0, synthBuffer.getNumSamples());
    
    for (int channel = 0; channel < outputBuffer.getNumChannels(); channel++)
    {
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);
        
        if (!adsr.isActive())
            clearCurrentNote();
    }
}
void SynthVoice::updateAlgorithm(int algo)
{
    algorithm = algo;
}

void SynthVoice::updateGain(float gainVal)
{
    gain.setGainDecibels (gainVal);
}

void SynthVoice::reset()
{
    gain.reset();
    adsr.reset();
}

void SynthVoice::updateLFO(int pmd, int amd, float freq, int lfoType)
{
    for (int ch = 0; ch < numChannelsToProcess; ch++)
    {
        lfo[ch].setFrequency(freq);
        lfoPMD = float(pmd) / 99;
        lfoAMD = float(amd) / 99;
        if (lfoType == 0)
            lfo[ch].initialise ([](float x) { return x / juce::MathConstants<float>::pi; });
        if (lfoType == 1)
            lfo[ch].initialise ([](float x) { return x < 0.0f ? -1.0f : 1.0f; });
        if (lfoType == 2)
            lfo[ch].initialise ([](float x) { return (x < 0) ? juce::jmap (x, -juce::MathConstants<float>::pi, 0.0f, -1.0f, 1.0f) : juce::jmap (x, 0.0f, juce::MathConstants<float>::pi, 1.0f, -1.0f); });
        if (lfoType == 3)
            lfo[ch].initialise ([](float x) { return 1; });
    }
}

