/*
  ==============================================================================

    OscComponent.h
    Created: 22 Apr 2025 9:06:55pm
    Author:  florentspagni

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class OscComponent  : public juce::Component
{
public:
    OscComponent(juce::AudioProcessorValueTreeState& apvts, juce::String waveSelectorID, juce::String fmFreqSelectorID, juce::String fmDepthSelectorID);
    ~OscComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    
    juce::ComboBox oscWaveSelector;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment>     oscWaveSelectorAttachment;
    
    juce::Slider fmFreqSlider;
    juce::Slider fmDepthSlider;
    
    using Attachment =juce::AudioProcessorValueTreeState::SliderAttachment;
    
    std::unique_ptr<Attachment>     fmFreqAttachement;
    juce::Label fmFreqLabel{"FMFreqLabel", "FM Freq"};
    
    std::unique_ptr<Attachment>     fmDepthAttachement;
    juce::Label fmDepthLabel{"FMDepthLabel", "FM Depth"};
    
    juce::Label waveSelectorLabel{"WaveSelectorLabel", "Wave type"};

    void setSliderWithLabel(juce::Slider& sidler, juce::Label& label, juce::AudioProcessorValueTreeState& apvts, juce::String paramID, std::unique_ptr<Attachment>& attachment ) ;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OscComponent)
};
