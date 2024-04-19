/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/
//NOTE: if an exception in thrown in juce_phase.h while testing the plugin, try uncommenting line 49 of juce_phase.h, it was the only solution I could find
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
KeelanSynthAudioProcessor::KeelanSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts (*this, nullptr, "Parameters", createParams())
#endif
{
    synth.addSound(new SynthSound());
    for (auto i = 0; i <= 8; ++i)
        synth.addVoice(new SynthVoice());
}

KeelanSynthAudioProcessor::~KeelanSynthAudioProcessor()
{
}

//==============================================================================
const juce::String KeelanSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool KeelanSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool KeelanSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool KeelanSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double KeelanSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int KeelanSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int KeelanSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void KeelanSynthAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String KeelanSynthAudioProcessor::getProgramName (int index)
{
    return {};
}

void KeelanSynthAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void KeelanSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    
    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->prepareToPlay(sampleRate, samplesPerBlock, getTotalNumOutputChannels(), 0);
        }
    }
    
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = getTotalNumOutputChannels();
    
}

void KeelanSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool KeelanSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void KeelanSynthAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    setParams();
    
    synth.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());
    juce::dsp::AudioBlock<float> block { buffer };
    
}

//==============================================================================
bool KeelanSynthAudioProcessor::hasEditor() const
{
    return true;
}

juce::AudioProcessorEditor* KeelanSynthAudioProcessor::createEditor()
{
    return new KeelanSynthAudioProcessorEditor (*this);
}

//==============================================================================
void KeelanSynthAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
}

void KeelanSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new KeelanSynthAudioProcessor();
}
//Where adjustable parameters are initialized
juce::AudioProcessorValueTreeState::ParameterLayout KeelanSynthAudioProcessor::createParams()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    // OSC select
    params.push_back (std::make_unique<juce::AudioParameterChoice> ("OSC1", "Oscillator 1", juce::StringArray { "Sine", "Saw", "Square" }, 0));
    params.push_back (std::make_unique<juce::AudioParameterChoice> ("OSC2", "Oscillator 2", juce::StringArray { "Sine", "Saw", "Square" }, 0));
    params.push_back (std::make_unique<juce::AudioParameterChoice> ("OSC3", "Oscillator 3", juce::StringArray { "Sine", "Saw", "Square" }, 0));
    params.push_back (std::make_unique<juce::AudioParameterChoice> ("OSC4", "Oscillator 4", juce::StringArray { "Sine", "Saw", "Square" }, 0));
    
    
    // OSC Gain
    params.push_back (std::make_unique<juce::AudioParameterFloat>("GAIN", "Global Gain", juce::NormalisableRange<float> { -60.0f, 0.2f, 0.1f , 1.5f}, 0.0f, "dB"));
    
    
    // FM Algorithm
    params.push_back (std::make_unique<juce::AudioParameterChoice> ("ALGORITHM", "FM Algorithm", juce::StringArray { "1","2","3","4","5","6","7","8" }, 0));
    
    juce::StringArray ratios = juce::StringArray("0.50", "0.71", "0.78", "0.87", "1.00", "1.41",
                                                 "1.57", "1.73", "2.00", "2.82", "3.00", "3.14",
                                                 "3.46", "4.00", "4.24", "4.71", "5.00", "5.19",
                                                 "5.65", "6.00", "6.28", "6.92", "7.00", "7.07",
                                                 "7.85", "8.00", "8.48", "8.65", "9.00", "9.42",
                                                 "9.89", "10.00", "10.38", "10.99", "11.00", "11.30",
                                                 "12.00", "12.11", "12.56", "12.72", "13.00", "13.84",
                                                 "14.00", "14.10", "14.13", "15.00", "15.55", "15.57",
                                                 "15.70", "16.96", "17.27", "17.30", "18.37", "18.84",
                                                 "19.03", "19.78", "20.41", "20.76", "21.20", "21.98",
                                                 "22.49", "23.55", "24.22", "25.95");
    
    params.push_back (std::make_unique<juce::AudioParameterChoice>("OSC1FMRATIO", "Oscillator 1 FM Ratio", ratios, 4));
    params.push_back (std::make_unique<juce::AudioParameterChoice>("OSC2FMRATIO", "Oscillator 2 FM Ratio", ratios, 4));
    params.push_back (std::make_unique<juce::AudioParameterChoice>("OSC3FMRATIO", "Oscillator 3 FM Ratio", ratios, 4));
    params.push_back (std::make_unique<juce::AudioParameterChoice>("OSC4FMRATIO", "Oscillator 4 FM Ratio", ratios, 4));
    
    // FM Osc Depth
    params.push_back (std::make_unique<juce::AudioParameterFloat>("OSC1FMDEPTH", "Oscillator 1 FM Depth", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 0.0f, ""));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("OSC2FMDEPTH", "Oscillator 2 FM Depth", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 0.0f, ""));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("OSC3FMDEPTH", "Oscillator 3 FM Depth", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 0.0f, ""));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("OSC4FMDEPTH", "Oscillator 4 FM Depth", juce::NormalisableRange<float> { 0.0f, 1.0f, 0.01f }, 0.0f, ""));
    
    // FM Osc Feedback - CURRENTLY NOT IN USE
    params.push_back (std::make_unique<juce::AudioParameterFloat>("OSC4FEEDBACK", "Oscillator 4 Feedback", juce::NormalisableRange<float> { 0.0f, 7.0f, 1.0f }, 0.0f, ""));
    
    // LFO
    params.push_back (std::make_unique<juce::AudioParameterFloat>("LFO1FREQ", "LFO1 Frequency", juce::NormalisableRange<float> { 0.0f, 99.0f, 1.0f }, 0.0f, ""));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("LFO1PMD", "LFO1 PMD", juce::NormalisableRange<float> { 0.0f, 99.0f, 1.0f}, 0.0f, ""));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("LFO1AMD", "LFO1 AMD", juce::NormalisableRange<float> { 0.0f, 99.0f, 1.0f}, 0.0f, ""));
    params.push_back (std::make_unique<juce::AudioParameterChoice>("LFO1WAVETYPE", "LFO1 Wave Type", juce::StringArray { "Saw", "Square", "Triangle", "Sample / Hold" }, 0));
    
    
    // ADSR
    params.push_back (std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float> { 0.01f, 1.0f, 0.01f }, 0.1f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float> { 0.01f, 1.0f, 0.01f }, 0.1f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float> { 0.01f, 1.0f, 0.01f }, 1.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", juce::NormalisableRange<float> { 0.01f, 3.0f, 0.01f }, 0.4f));
    
    return { params.begin(), params.end() };
}


void KeelanSynthAudioProcessor::setParams()
{
    //where parameters are assigned to objects
    float ratios[64] = {0.50f, 0.71f, 0.78f, 0.87f, 1.00f, 1.41f,
        1.57f, 1.73f, 2.00f, 2.82f, 3.00f, 3.14f,
        3.46f, 4.00f, 4.24f, 4.71f, 5.00f, 5.19f,
        5.65f, 6.00f, 6.28f, 6.92f, 7.00f, 7.07f,
        7.85f, 8.00f, 8.48f, 8.65f, 9.00f, 9.42f,
        9.89f, 10.00f, 10.38f, 10.99f, 11.00f, 11.30f,
        12.00f, 12.11f, 12.56f, 12.72f, 13.00f, 13.84f,
        14.00f, 14.10f, 14.13f, 15.00f, 15.55f, 15.57f,
        15.70f, 16.96f, 17.27f, 17.30f, 18.37f, 18.84f,
        19.03f, 19.78f, 20.41f, 20.76f, 21.20f, 21.98f,
        22.49f, 23.55f, 24.22f, 25.95f};
    for (int i = 0; i < synth.getNumVoices(); ++i)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            auto& attack = *apvts.getRawParameterValue ("ATTACK");
            auto& decay = *apvts.getRawParameterValue ("DECAY");
            auto& sustain = *apvts.getRawParameterValue ("SUSTAIN");
            auto& release = *apvts.getRawParameterValue ("RELEASE");
            
            auto& osc1Choice = *apvts.getRawParameterValue ("OSC1");
            auto& osc2Choice = *apvts.getRawParameterValue ("OSC2");
            auto& osc3Choice = *apvts.getRawParameterValue ("OSC3");
            auto& osc4Choice = *apvts.getRawParameterValue ("OSC4");
            
            auto& gain = *apvts.getRawParameterValue ("GAIN");
            
            auto& lfoFrequency = *apvts.getRawParameterValue ("LFO1FREQ");
            auto& lfoPMD = *apvts.getRawParameterValue ("LFO1PMD");
            auto& lfoAMD = *apvts.getRawParameterValue ("LFO1AMD");
            auto& lfoType = *apvts.getRawParameterValue ("LFO1WAVETYPE");
            
            auto& osc1FmRatio = *apvts.getRawParameterValue ("OSC1FMRATIO");
            auto& osc2FmRatio = *apvts.getRawParameterValue ("OSC2FMRATIO");
            auto& osc3FmRatio = *apvts.getRawParameterValue ("OSC3FMRATIO");
            auto& osc4FmRatio = *apvts.getRawParameterValue ("OSC4FMRATIO");
            
            auto& osc1FmDepth = *apvts.getRawParameterValue ("OSC1FMDEPTH");
            auto& osc2FmDepth = *apvts.getRawParameterValue ("OSC2FMDEPTH");
            auto& osc3FmDepth = *apvts.getRawParameterValue ("OSC3FMDEPTH");
            auto& osc4FmDepth = *apvts.getRawParameterValue ("OSC4FMDEPTH");
            
            auto& osc4FmFeedback = *apvts.getRawParameterValue ("OSC4FEEDBACK");
            
            auto& alg = *apvts.getRawParameterValue ("ALGORITHM");

            auto& osc1 = voice->getOscillator1();
            auto& osc2 = voice->getOscillator2();
            auto& osc3 = voice->getOscillator3();
            auto& osc4 = voice->getOscillator4();
            
            auto& adsr = voice->getAdsr();
            
            if (lfoFrequency < 1) {
                lfoFreq = 0.06;
            } else {
                lfoFreq = lfoFrequency / 2;
            }
            algorithm = int(alg) + 1;
            voice->updateAlgorithm(algorithm);
            voice->updateGain(gain);
            voice->updateLFO(lfoPMD, lfoAMD, lfoFreq, lfoType);
           
            for (int i = 0; i < getTotalNumOutputChannels(); i++)
            {
                switch (algorithm) {
                        
                    case 1:
                        osc1[i].setParams (osc1Choice, 0, ratios[int(osc1FmRatio)], osc1FmDepth, ratios[int(osc2FmRatio)], osc2FmDepth, ratios[int(osc3FmRatio)], osc3FmDepth, ratios[int(osc4FmRatio)], osc4FmDepth, osc4FmFeedback, 1);
                        break;
                        
                    case 2:
                        osc1[i].setParams (osc1Choice, 0, ratios[int(osc1FmRatio)], osc1FmDepth, ratios[int(osc2FmRatio)], osc2FmDepth, ratios[int(osc3FmRatio)], osc3FmDepth, ratios[int(osc4FmRatio)], osc4FmDepth, osc4FmFeedback, 2);
                        break;
                        
                    case 3:
                        osc1[i].setParams (osc1Choice, 0, ratios[int(osc1FmRatio)], osc1FmDepth, ratios[int(osc2FmRatio)], osc2FmDepth, ratios[int(osc3FmRatio)], osc3FmDepth, ratios[int(osc4FmRatio)], osc4FmDepth, osc4FmFeedback, 3);
                        break;
                        
                    case 4:
                        osc1[i].setParams (osc1Choice, 0, ratios[int(osc1FmRatio)], osc1FmDepth, ratios[int(osc2FmRatio)], osc2FmDepth, ratios[int(osc3FmRatio)], osc3FmDepth, ratios[int(osc4FmRatio)], osc4FmDepth, osc4FmFeedback, 4);
                        break;
                        
                    case 5:
                        osc1[i].setParams (osc1Choice, 0, ratios[int(osc1FmRatio)], osc1FmDepth, ratios[int(osc2FmRatio)], osc2FmDepth, ratios[int(osc3FmRatio)], osc3FmDepth, ratios[int(osc4FmRatio)], osc4FmDepth, osc4FmFeedback, 5);
                        osc3[i].setParams (osc3Choice, 0, ratios[int(osc3FmRatio)], osc3FmDepth, ratios[int(osc2FmRatio)], osc2FmDepth, ratios[int(osc3FmRatio)], osc3FmDepth, ratios[int(osc4FmRatio)], osc4FmDepth, osc4FmFeedback, 6);
                        break;
                        
                    case 6:
                        osc1[i].setParams (osc1Choice, 0, ratios[int(osc1FmRatio)], osc1FmDepth, ratios[int(osc2FmRatio)], osc2FmDepth, ratios[int(osc3FmRatio)], osc3FmDepth, ratios[int(osc4FmRatio)], osc4FmDepth, osc4FmFeedback, 6);
                        osc2[i].setParams (osc2Choice, 0, ratios[int(osc2FmRatio)], osc2FmDepth, ratios[int(osc2FmRatio)], osc2FmDepth, ratios[int(osc3FmRatio)], osc3FmDepth, ratios[int(osc4FmRatio)], osc4FmDepth, osc4FmFeedback, 6);
                        osc3[i].setParams (osc3Choice, 0, ratios[int(osc3FmRatio)], osc3FmDepth, ratios[int(osc2FmRatio)], osc2FmDepth, ratios[int(osc3FmRatio)], osc3FmDepth, ratios[int(osc4FmRatio)], osc4FmDepth, osc4FmFeedback, 6);
                        break;
                        
                    case 7:
                        osc1[i].setParams (osc1Choice, 0, ratios[int(osc1FmRatio)], osc1FmDepth, ratios[int(osc2FmRatio)], osc2FmDepth, ratios[int(osc3FmRatio)], osc3FmDepth, ratios[int(osc4FmRatio)], osc4FmDepth, osc4FmFeedback, 7);
                        osc2[i].setParams (osc2Choice, 0, ratios[int(osc2FmRatio)], osc2FmDepth, ratios[int(osc2FmRatio)], osc2FmDepth, ratios[int(osc3FmRatio)], osc3FmDepth, ratios[int(osc4FmRatio)], osc4FmDepth, osc4FmFeedback, 7);
                        osc3[i].setParams (osc3Choice, 0, ratios[int(osc3FmRatio)], osc3FmDepth, ratios[int(osc2FmRatio)], osc2FmDepth, ratios[int(osc3FmRatio)], osc3FmDepth, ratios[int(osc4FmRatio)], osc4FmDepth, osc4FmFeedback, 6);
                        break;
                        
                    case 8:
                        osc1[i].setParams (osc1Choice, 0, ratios[int(osc1FmRatio)], osc1FmDepth, ratios[int(osc2FmRatio)], osc2FmDepth, ratios[int(osc3FmRatio)], osc3FmDepth, ratios[int(osc4FmRatio)], osc4FmDepth, osc4FmFeedback, 7);
                        osc2[i].setParams (osc2Choice, 0, ratios[int(osc2FmRatio)], osc2FmDepth, ratios[int(osc2FmRatio)], osc2FmDepth, ratios[int(osc3FmRatio)], osc3FmDepth, ratios[int(osc4FmRatio)], osc4FmDepth, osc4FmFeedback, 7);
                        osc3[i].setParams (osc3Choice, 0, ratios[int(osc3FmRatio)], osc3FmDepth, ratios[int(osc2FmRatio)], osc2FmDepth, ratios[int(osc3FmRatio)], osc3FmDepth, ratios[int(osc4FmRatio)], osc4FmDepth, osc4FmFeedback, 7);
                        osc4[i].setParams (osc4Choice, 0, ratios[int(osc4FmRatio)], osc4FmDepth, ratios[int(osc2FmRatio)], osc2FmDepth, ratios[int(osc3FmRatio)], osc3FmDepth, ratios[int(osc4FmRatio)], osc4FmDepth, osc4FmFeedback, 8);
                        break;
                        
                    default:
                        jassertfalse;
                        break;
                }
            }
            adsr.update (attack.load(), decay.load(), sustain.load(), release.load());
        }
    }
}

