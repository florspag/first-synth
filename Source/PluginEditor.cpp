/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FirstSynthAudioProcessorEditor::FirstSynthAudioProcessorEditor (FirstSynthAudioProcessor& p)
: AudioProcessorEditor (&p),
audioProcessor (p), osc(audioProcessor.apvts, "Osc1WaveType", "Osc1FMFreq", "Osc1FMDepth") ,
adsr(audioProcessor.apvts, "Envelope", "ATTACK", "DECAY", "SUSTAIN", "RELEASE"),
modAdsr(audioProcessor.apvts, "Mod Envelope",  "MODAttack", "MODDecay", "MODSustain", "MODRelease"),
filter(audioProcessor.apvts, "FilterType", "FilterCutOff", "FilterRes")
{
    
    
    setSize (500, 620);
    
    addAndMakeVisible(osc);
    addAndMakeVisible(adsr);
    addAndMakeVisible(filter);
    addAndMakeVisible(modAdsr);
    
}

FirstSynthAudioProcessorEditor::~FirstSynthAudioProcessorEditor()
{
}

//==============================================================================
void FirstSynthAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);
}

void FirstSynthAudioProcessorEditor::resized()
{
    
    const auto paddingX = 10;
    const auto paddingY = 35;
    const auto paddingY2 = 235;
    
    const auto oscWidth = 200;
    const auto oscHeight = 200;
    
    osc.setBounds(paddingX, paddingY, oscWidth, oscHeight);
    adsr.setBounds(osc.getRight() + paddingX, paddingY , 280, 200);
    filter.setBounds(paddingX, paddingY2, oscWidth, oscHeight);
    modAdsr.setBounds(filter.getRight() + paddingX, paddingY2, 280, 200);
}


