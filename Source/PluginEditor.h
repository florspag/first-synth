/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "UI/AdsrComponent.h"
#include "UI/OscComponent.h"
#include "UI/FilterComponent.h"

//==============================================================================
/**
*/
class FirstSynthAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    FirstSynthAudioProcessorEditor (FirstSynthAudioProcessor&);
    ~FirstSynthAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:



    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    std::unique_ptr<SliderAttachment> gainSliderAttachement;
    
    FirstSynthAudioProcessor& audioProcessor;
    OscComponent osc;
    AdsrComponent adsr;
    AdsrComponent modAdsr;
    FilterComponent filter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FirstSynthAudioProcessorEditor)
};
