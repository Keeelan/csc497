/*
  ==============================================================================

    OscData.cpp
    Created: 29 Feb 2024 1:08:58pm
    Author:  Keelan Siemens

  ==============================================================================
*/

#include "OscData.h"


void OscData::prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels)
{
    resetAll();
    
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
    
    prepare(spec);
    fm1Osc.prepare(spec);
    fm2Osc.prepare(spec);
    fm3Osc.prepare(spec);
}

void OscData::setWaveType(const int choice)
{
    // return std::sin (x); //Sine wave
    // return x / juce::MathConstants<float>::pi; //Saw wave
    // return x < 0.0f ? -1.0f : 1.0f; //Square wave
    
    switch  (choice) {
        case 0:
            //sine wave
            initialise([](float x) { return std::sin (x);});
            break;
            
        case 1:
            //saw wave
            initialise([](float x) { return x / juce::MathConstants<float>::pi;});
            break;
            
        case 2:
            //square wave
            initialise([](float x) { return x < 0.0f ? -1.0f : 1.0f; });
            break;
            
        default:
            jassertfalse;
            break;
    }
}

void OscData::setOscPitch (const int pitch)
{
    lastPitch = pitch;
    setFrequency (juce::MidiMessage::getMidiNoteInHertz ((lastMidiNote)) * oscRatio + fmMod);

}

void OscData::setFreq(const int midiNoteNumber)
{
    lastMidiNote = midiNoteNumber;
}

void OscData::setFmOsc (const float ratio, const float depth, const float ratio1, const float depth1, const float ratio2, const float depth2, const float ratio3, const float depth3)
{
    fm3Ratio = ratio3;
    fm3Depth = depth3 * 100 * fm3Ratio * juce::MidiMessage::getMidiNoteInHertz (lastMidiNote);
    fm2Ratio = ratio2;
    fm2Depth = depth2 * 100 * fm2Ratio * juce::MidiMessage::getMidiNoteInHertz (lastMidiNote);
    fm1Ratio = ratio1;
    fm1Depth = depth1 * 100 * fm1Ratio * juce::MidiMessage::getMidiNoteInHertz (lastMidiNote);
    oscRatio = ratio;
    oscDepth = depth;
}

float OscData::processNextSample (float input, float lfoValue)
{
    float freq = 1;
    lfoValue = lfoValue * 12;
    float lfoFreq = 440 * std::pow (2.0, (lastMidiNote + lfoValue - 69) / 12.0);
    switch (algorithm) {
        case 1:
            //ALG 1
            fm3Osc.setFrequency (juce::MidiMessage::getMidiNoteInHertz (lastMidiNote) * fm3Ratio);
            fm3Mod = (fm3Osc.processSample (input) * fm3Depth);
            fm2Osc.setFrequency ((juce::MidiMessage::getMidiNoteInHertz (lastMidiNote) + fm3Mod) * fm2Ratio);
            fm2Mod = (fm2Osc.processSample (input) * fm2Depth);
            fm1Osc.setFrequency ((juce::MidiMessage::getMidiNoteInHertz (lastMidiNote) + fm2Mod) * fm1Ratio);
            fm1Mod = (fm1Osc.processSample (input) * fm1Depth);
            fmMod = fm1Mod;
            freq = ((lfoFreq + fmMod) * oscRatio);
            setFrequency (freq);
            return ((processSample (input)) * oscDepth);
            
        case 2:
            //ALG 2
            fm3Osc.setFrequency (juce::MidiMessage::getMidiNoteInHertz (lastMidiNote) * fm3Ratio);
            fm3Mod = (fm3Osc.processSample (input) * fm3Depth);
            fm2Osc.setFrequency ((juce::MidiMessage::getMidiNoteInHertz (lastMidiNote)) * fm2Ratio);
            fm2Mod = (fm2Osc.processSample (input) * fm2Depth);
            fm1Osc.setFrequency ((juce::MidiMessage::getMidiNoteInHertz (lastMidiNote) + fm2Mod + fm3Mod) * fm1Ratio);
            fm1Mod = (fm1Osc.processSample (input) * fm1Depth);
            fmMod = fm1Mod;
            freq = ((lfoFreq + fmMod) * oscRatio);
            setFrequency (freq);
            return ((processSample (input)) * oscDepth);
            
        case 3:
            //ALG 3
            fm3Osc.setFrequency (juce::MidiMessage::getMidiNoteInHertz (lastMidiNote) * fm3Ratio);
            fm3Mod = (fm3Osc.processSample (input) * fm3Depth);
            fm2Osc.setFrequency ((juce::MidiMessage::getMidiNoteInHertz (lastMidiNote)) * fm2Ratio);
            fm2Mod = (fm2Osc.processSample (input) * fm2Depth);
            fm1Osc.setFrequency ((juce::MidiMessage::getMidiNoteInHertz (lastMidiNote) + fm2Mod) * fm1Ratio);
            fm1Mod = (fm1Osc.processSample (input) * fm1Depth);
            fmMod = fm1Mod + fm3Mod;
            freq = ((lfoFreq + fmMod) * oscRatio);
            setFrequency (freq);
            return ((processSample (input)) * oscDepth);
            
        case 4:
            //ALG 4
            fm3Osc.setFrequency (juce::MidiMessage::getMidiNoteInHertz (lastMidiNote) * fm3Ratio);
            fm3Mod = (fm3Osc.processSample (input) * fm3Depth);
            fm2Osc.setFrequency ((juce::MidiMessage::getMidiNoteInHertz (lastMidiNote) + fm3Mod) * fm2Ratio);
            fm2Mod = (fm2Osc.processSample (input) * fm2Depth);
            fm1Osc.setFrequency ((juce::MidiMessage::getMidiNoteInHertz (lastMidiNote)) * fm1Ratio);
            fm1Mod = (fm1Osc.processSample (input) * fm1Depth);
            fmMod = fm1Mod + fm2Mod;
            freq = ((lfoFreq + fmMod) * oscRatio);
            setFrequency (freq);
            return ((processSample (input)) * oscDepth);
            
        case 5:
            //ALG 5 CARRIER OSC TYPE 1
            fm1Osc.setFrequency ((juce::MidiMessage::getMidiNoteInHertz (lastMidiNote)) * fm1Ratio);
            fm1Mod = (fm1Osc.processSample (input) * fm1Depth);
            fmMod = fm1Mod;
            freq = ((lfoFreq + fmMod) * oscRatio);
            setFrequency (freq);
            return ((processSample (input)) * oscDepth);
            
        case 6:
            //ALG 5 CARRIER OSC TYPE 2 - ALG 6 CARRIER OSC TYPE 1 - ALG 7 CARRIER OSC TYPE 2
            fm3Osc.setFrequency (juce::MidiMessage::getMidiNoteInHertz (lastMidiNote) * fm3Ratio);
            fm3Mod = (fm3Osc.processSample (input) * fm3Depth);
            fm3Osc.setFrequency ((juce::MidiMessage::getMidiNoteInHertz (lastMidiNote) + (fm3Mod * feedback)) * fm3Ratio);
            fm3Mod = (fm3Osc.processSample (input) * fm3Depth);
            fmMod = fm3Mod;
            freq = ((lfoFreq + fmMod) * oscRatio);;
            setFrequency (freq);
            return ((processSample (input)) * oscDepth);
            
        case 7:
            //ALG 7 CARRIER OSC TYPE 1 - ALG 8 CARRIER OSC TYPE 1
            freq = (lfoFreq * oscRatio);
            setFrequency (freq);
            return ((processSample (input)) * oscDepth);
            
        case 8:
            // ALG 8 FEEDBACK OSCILLATOR
            // WARNING: NOT FUNCTIONAL
            fmMod = (lastSampleFeedbackOsc * feedback * 100);
            freq = ((lfoFreq + fmMod) * oscRatio);
            lastSampleFeedbackOsc = ((processSample (input)) * oscDepth);
            return ((processSample (input)) * oscDepth);
            
        default:
            jassertfalse;
            fm3Mod = fm3Osc.processSample (input) * fm3Depth;
            fm2Mod = (fm2Osc.processSample (input) + fm3Mod) * fm2Depth;
            fm1Mod = (fm1Osc.processSample (input) + fm2Mod) * fm1Depth;
            fmMod = fm1Mod;
            return ((processSample (processSample (input)) + fmMod) * oscDepth);
            break;
    }
}

void OscData::setParams (const int oscChoice, const int oscPitch, const float oscRatio, const float oscDepth, const float fm1Ratio, const float fm1Depth, const float fm2Ratio, const float fm2Depth, const float fm3Ratio, const float fm3Depth, const float fm3Feedback, const float alg)
{
    feedback = fm3Feedback / 7;
    algorithm = alg;
    setWaveType (oscChoice);
    setOscPitch (oscPitch);
    setFmOsc (oscRatio, oscDepth, fm1Ratio, fm1Depth, fm2Ratio, fm2Depth, fm3Ratio, fm3Depth);
}

void OscData::resetAll()
{
    reset();
    fm1Osc.reset();
    fm2Osc.reset();
    fm3Osc.reset();
}
