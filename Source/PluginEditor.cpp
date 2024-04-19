/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
KeelanSynthAudioProcessorEditor::KeelanSynthAudioProcessorEditor (KeelanSynthAudioProcessor& p)
: AudioProcessorEditor (&p)
, audioProcessor (p)
, osc1 (audioProcessor.apvts, "OSC1", "OSC1FMRATIO", "OSC1FMDEPTH", "OSC4FEEDBACK")
, osc2 (audioProcessor.apvts, "OSC2", "OSC2FMRATIO", "OSC2FMDEPTH", "OSC4FEEDBACK")
, osc3 (audioProcessor.apvts, "OSC3", "OSC3FMRATIO", "OSC3FMDEPTH", "OSC4FEEDBACK")
, osc4 (audioProcessor.apvts, "OSC4", "OSC4FMRATIO", "OSC4FMDEPTH", "OSC4FEEDBACK")
, alg (audioProcessor.apvts, "ALGORITHM")
, adsr (audioProcessor.apvts, "ATTACK", "DECAY", "SUSTAIN", "RELEASE")
, gain (audioProcessor.apvts, "GAIN")
, lfo1 (audioProcessor.apvts, "LFO1FREQ", "LFO1PMD", "LFO1AMD", "LFO1WAVETYPE")
{
    
    addAndMakeVisible (osc1);
    addAndMakeVisible (osc2);
    addAndMakeVisible (osc3);
    addAndMakeVisible (osc4);
    
    addAndMakeVisible (alg);
    addAndMakeVisible (lfo1);
    addAndMakeVisible (adsr);
    addAndMakeVisible (gain);
    
    osc1.setName ("Oscillator 1");
    osc2.setName ("Oscillator 2");
    osc3.setName ("Oscillator 3");
    osc4.setName ("Oscillator 4");
    
    alg.setName ("FM Algorithm");
    lfo1.setName ("Filter LFO");
    adsr.setName ("ADSR");
    gain.setName ("Global Gain");
    
    auto oscColour = juce::Colour::fromRGB (0, 249, 200);
    auto lfoColour = juce::Colour::fromRGB (246, 87, 64);
    
    osc1.setBoundsColour (oscColour);
    osc2.setBoundsColour (oscColour);
    osc3.setBoundsColour (oscColour);
    osc4.setBoundsColour (oscColour);
    
    alg.setBoundsColour (lfoColour);
    lfo1.setBoundsColour (lfoColour);
    
        
    
    startTimerHz (30);
    setSize (810, 530);
    
    
}

KeelanSynthAudioProcessorEditor::~KeelanSynthAudioProcessorEditor()
{
}

//==============================================================================
void KeelanSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
//    // (Our component is opaque, so we must completely fill the background with a solid colour)
//    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));
//
//    g.setColour (juce::Colours::white);
//    g.setFont (15.0f);
//    g.drawFittedText ("Hello World!", getLocalBounds(), juce::Justification::centred, 1);
    g.fillAll(juce::Colours::black);
}

void KeelanSynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    //set bounds
//    osc1.setBounds (10, 10, 200, 200);
//    osc2.setBounds (osc1.getRight(), 10, 200, 200);
////    osc3.setBounds (10, osc1.getBottom(), 200, 200);
////    osc4.setBounds (osc3.getRight(), osc1.getBottom(), 200, 200);
//    adsr.setBounds(getWidth() / 2, 0, getWidth() / 2, getHeight());
    
    const auto oscWidth = 280;
    const auto oscHeight = 132;
    osc1.setBounds (0, 0, oscWidth, oscHeight);
    osc2.setBounds (0, osc1.getBottom(), oscWidth, oscHeight);
    osc3.setBounds (0, osc2.getBottom(), oscWidth, oscHeight);
    osc4.setBounds (0, osc3.getBottom(), oscWidth, oscHeight);
    alg.setBounds (osc1.getRight(), 0, 180, 132);
    lfo1.setBounds (osc1.getRight(), alg.getBottom(), 180, 264);
    adsr.setBounds (lfo1.getRight(), 0, 230, osc4.getBottom());
    gain.setBounds (adsr.getRight(), 0, 115, osc4.getBottom());
    

}

void KeelanSynthAudioProcessorEditor::timerCallback()
{
    repaint();
}
